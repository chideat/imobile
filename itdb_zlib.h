#include "itdb.h"

#define CHUNK 16384

static int zlib_inflate(char *outbuf, char *zdata, size_t compressed_size, size_t *uncompressed_size)
{
    int ret;
    uint32 inpos = 0;
    uint32 outpos = 0;
    unsigned have;
    z_stream strm;
    unsigned char out[CHUNK];

    /* allocate inflate state */
    strm.zalloc = Z_NULL;
    strm.zfree = Z_NULL;
    strm.opaque = Z_NULL;
    strm.avail_in = 0;
    strm.next_in = Z_NULL;
    ret = inflateInit(&strm);
    if (ret != Z_OK)
        return ret;

    *uncompressed_size = 0;

    /* decompress until deflate stream ends or end of file */
    do {
        strm.avail_in = CHUNK;
    if (inpos+strm.avail_in > compressed_size) {
        strm.avail_in = compressed_size - inpos;
    }
        strm.next_in = (unsigned char*)zdata+inpos;
    inpos+=strm.avail_in;

        /* run inflate() on input until output buffer not full */
        do {
            strm.avail_out = CHUNK;
            if (outbuf)  {
                strm.next_out = (unsigned char*)(outbuf + outpos);
            } else {
                strm.next_out = out;
            }
            ret = inflate(&strm, Z_NO_FLUSH);
           // g_assert(ret != Z_STREAM_ERROR);  /* state not clobbered */
            switch (ret) {
            case Z_NEED_DICT:
                ret = Z_DATA_ERROR;     /* and fall through */
            case Z_DATA_ERROR:
            case Z_MEM_ERROR:
                (void)inflateEnd(&strm);
                return ret;
            }
            have = CHUNK - strm.avail_out;
        *uncompressed_size += have;
        if (outbuf) {
        outpos += have;
        }
        } while (strm.avail_out == 0);

        /* done when inflate() says it's done */
    } while (ret != Z_STREAM_END);

    /* clean up and return */
    (void)inflateEnd(&strm);
    return ret == Z_STREAM_END ? Z_OK : Z_DATA_ERROR;
}


int itdb_zlib_decompress(Itdb_Ringtone *itdb) {
    if (itdb == NULL || itdb->cts == NULL || itdb->cts->contents == NULL) {
        throw 1;
    }


    It_Content *cts = itdb->cts;
    unsigned int c_size = *(unsigned int*)(cts->contents + 8),
                 h_size = *(unsigned int*)(cts->contents + 4);
    size_t u_size = 0;

    if (h_size < 0xA9) {
#ifdef DEBUG
        fprintf(stderr, "header is too small for iTunesCDB!\n");
#endif
        throw 1;
    }
    if (*(uchar*)(cts->contents + 0xA8) == 1) {
        *(uchar*)(cts->contents + 0xA8) = 0;
    }
    else {
        fprintf(stderr, "Warning: unknown value for 0xa8 in header, should be 1 for uncompressed");
    }

    if (zlib_inflate(NULL, cts->contents + h_size, c_size - h_size, &u_size) == 0) {
        char *n_contents = new char[u_size + h_size];
        memcpy(n_contents, cts->contents, h_size);

        if (zlib_inflate(n_contents + h_size, cts->contents + h_size, c_size - h_size, &u_size) == 0) {
            delete cts->contents;
            cts->contents = n_contents;
            cts->length = u_size + h_size;
        }
        else
            throw 1;
    }
}


int itdb_zlib_compress (Itdb_Ringtone *itdb) {
    if (itdb == NULL || itdb->cts == NULL || itdb->cts->contents == NULL) {
        throw 1;
    }

    It_Content *cts = itdb->cts;
    uint32 header_len;
    uLongf compressed_len;
    uLongf uncompressed_len;
    char *new_contents;
    int status;

    header_len = *(uint32*)(cts->contents+4);
    uncompressed_len = cts->length - header_len;

    if (header_len < 0xA9) {
        fprintf(stderr, "Header is too small for iTunesCDB!\n");
        throw 1;
    }

    /* compression flag */
    if (*(uchar*)(cts->contents+0xa8) == 0) {
	    *(uchar*)(cts->contents+0xa8) = 1;
    }
    else {
	    fprintf (stderr, "Unknown value for 0xa8 in header: should be 0 for uncompressed, is %d.\n", *(uchar*)(cts->contents+0xa8));
    }

    compressed_len = compressBound (uncompressed_len);

    new_contents = new char[header_len + compressed_len];
    memcpy (new_contents, cts->contents, header_len);
    status = compress2 ((uchar*)new_contents + header_len, &compressed_len, (uchar*)cts->contents + header_len, uncompressed_len, 1);
    if (status != Z_OK) {
	    delete new_contents;
#ifdef DEBUG
		fprintf(stderr, "Error compressing iTunesCDB file!\n");
#endif
        throw 1;
    }
    delete cts->contents;
    /* update mhbd size */
    *(uint32*)(new_contents+8) = compressed_len + header_len;
    cts->contents = new_contents;
    cts->length = compressed_len + header_len;

    return 1;
}
