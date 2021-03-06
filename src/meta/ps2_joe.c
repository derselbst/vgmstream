#include "meta.h"
#include "../util.h"

/* JOE (found in Wall-E and some more Pixar games) */
VGMSTREAM * init_vgmstream_ps2_joe(STREAMFILE *streamFile) {
    VGMSTREAM * vgmstream = NULL;
    char filename[PATH_LIMIT];
    off_t start_offset;
	uint8_t	testBuffer[0x10];
	off_t	loopStart = 0;
	off_t	loopEnd = 0;
	off_t	readOffset = 0;
	off_t	blockOffset = 0;
	off_t	sampleOffset = 0;
	size_t	fileLength;
	size_t	dataLength;
	size_t	dataInterleave;
    int loop_flag;
	int channel_count;

    /* check extension, case insensitive */
    streamFile->get_name(streamFile,filename,sizeof(filename));
    if (strcasecmp("joe",filename_extension(filename))) goto fail;

    /* check header */
    // if (read_32bitBE(0x0C,streamFile) != 0xCCCCCCCC)
    //    goto fail;

    loop_flag = 1;
    channel_count = 2;

	/* build the VGMSTREAM */
    vgmstream = allocate_vgmstream(channel_count,loop_flag);
    if (!vgmstream) goto fail;

	fileLength = get_streamfile_size(streamFile);
	dataLength = read_32bitLE(0x4,streamFile);
	dataInterleave = read_32bitLE(0x8,streamFile);

	if (!dataInterleave)
		dataInterleave = 16; /* XXX */

	/* fill in the vital statistics */
    start_offset = fileLength - dataLength;
	vgmstream->channels = channel_count;
    vgmstream->sample_rate = read_32bitLE(0x0,streamFile);
    vgmstream->coding_type = coding_PSX;
    vgmstream->num_samples = dataLength*28/16/channel_count;
		
	
    readOffset = start_offset;
	do {
		off_t blockRead = (off_t)read_streamfile(testBuffer,readOffset,0x10,streamFile);

		readOffset += blockRead;
		blockOffset += blockRead;

		if (blockOffset >= dataInterleave) {
			readOffset += dataInterleave;
			blockOffset -= dataInterleave;
		}
		
		/* Loop Start */
		if(testBuffer[0x01]==0x06) {
			if(loopStart == 0) loopStart = sampleOffset;
			/* break; */
		}

		sampleOffset += 28;

		/* Loop End */
		if(testBuffer[0x01]==0x03) {
			if(loopEnd == 0) loopEnd = sampleOffset;
			/* break; */
		}

	} while (streamFile->get_offset(streamFile)<(int32_t)fileLength);
	
	if(loopStart == 0 && loopEnd == 0) {
		loop_flag = 0;
		vgmstream->num_samples = dataLength*28/16/channel_count;
	} else {
		loop_flag = 1;
		vgmstream->loop_start_sample = loopStart;
        vgmstream->loop_end_sample = loopEnd;
    }

	vgmstream->layout_type = layout_interleave;
	vgmstream->interleave_block_size = dataInterleave;
	vgmstream->meta_type = meta_PS2_JOE;

    /* open the file for reading */
    {
        int i;
        STREAMFILE * file;
        file = streamFile->open(streamFile,filename,STREAMFILE_DEFAULT_BUFFER_SIZE);
        if (!file) goto fail;
        for (i=0;i<channel_count;i++) {
            vgmstream->ch[i].streamfile = file;

            vgmstream->ch[i].channel_start_offset=
                vgmstream->ch[i].offset=start_offset+
                vgmstream->interleave_block_size*i;

        }
    }

    return vgmstream;

    /* clean up anything we may have opened */
fail:
    if (vgmstream) close_vgmstream(vgmstream);
    return NULL;
}
