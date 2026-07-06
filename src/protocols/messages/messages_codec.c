#include <internal/protocols/messages/messages_codec.h>

#include <internal/protocols/messages/codec_utility.h>

bool mm_encode(const ENCONDED_DATA_MASK *mask,
    const mnn_data_t    *data,
    uint8_t *out, size_t out_size,
    size_t  *encoded_len)
{
    if (out == NULL || data == NULL || encoded_len == NULL) return false;

    *encoded_len = 0;

    if(*mask & MEDM_NONE) return true;

    encoder_t enc;
    mc_encoder_setup(&enc,out,out_size);

    bool retVal = true;

    if(true) // we do it anyway to avoid race conditions in decoding phase
    {
        retVal = mc_encode_u32(&enc,*mask);
        if(retVal == false) return false;
    }

    if(*mask & MEDM_PID )
    {
        retVal = mc_encode_u32(&enc,data->pid);
        if(retVal == false) return false;
    }

    if(*mask & MEDM_THREAD)
    {
        retVal = mc_encode_u64(&enc,data->thread_id);
        if(retVal == false) return false;
    }

    if(*mask & MEDM_TIMESTAMP)
    {
        retVal = mc_encode_u64(&enc,data->timestamp);
        if(retVal == false) return false;
    }

    if(*mask & MEDM_FILE)
    {
        retVal = mc_encode_u8(&enc,data->file_len);
        if(retVal == false) return false;
        
        retVal = mc_encode_bytes(&enc,data->file,data->file_len);
        if(retVal == false) return false;
    }

    if(*mask & MEDM_LINE)
    {
        retVal = mc_encode_u32(&enc,data->line);
        if(retVal == false) return false;
    }

    if(*mask & MEDM_SEVERITY)
    {
        retVal = mc_encode_u8(&enc,data->severity);
        if(retVal == false) return false;
    }

    if(*mask & MEDM_FUNCTION)
    {
        retVal = mc_encode_u8(&enc,data->func_len);
        if(retVal == false) return false;

        retVal = mc_encode_bytes(&enc,data->func,data->func_len);
        if(retVal == false) return false;
    }

    if(*mask & MEDM_MESSAGE)
    {
        retVal = mc_encode_u16(&enc,data->msg_len);
        if(retVal == false) return false;

        retVal = mc_encode_bytes(&enc,data->msg,data->msg_len);
        if(retVal == false) return false;
    }

    if (encoded_len) 
    {
        *encoded_len = enc.current_size; 
    }

    return true;
}

bool mm_decode(mnn_data_t *out,
    const uint8_t *in, size_t  in_size,
    size_t *decoded_bytes)
{
    if( out == NULL || in == NULL || in_size == 0 || decoded_bytes == NULL ) return false;

    *decoded_bytes = 0;

    bool retVal = true;
    
    decoder_t dec;
    mc_decoder_setup(&dec,in,in_size);

    ENCONDED_DATA_MASK mask;
    mc_decode_u32(&dec,&mask);

    

    return true;

}
