#include <internal/protocols/messages/messages_codec.h>

#include <internal/protocols/messages/codec_utility.h>

bool mm_encode(const ENCODED_DATA_MASK *mask,
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
    const uint8_t *in, size_t  in_size)
{
    if( out == NULL || in == NULL || in_size == 0 ) return false;

    bool retVal = true;
    
    decoder_t dec;
    mc_decoder_setup(&dec,in,in_size);

    ENCODED_DATA_MASK mask;
    mc_decode_u32(&dec,&mask);

    if(mask & MEDM_PID )
    {
        retVal = mc_decode_u32(&dec,&out->pid);
        if(retVal == false) return false;
    }

    if(mask & MEDM_THREAD)
    {
        retVal = mc_decode_u64(&dec,&out->thread_id);
        if(retVal == false) return false;
    }

    if(mask & MEDM_TIMESTAMP)
    {
        retVal = mc_decode_u64(&dec,&out->timestamp);
        if(retVal == false) return false;
    }

    if(mask & MEDM_FILE)
    {
        retVal = mc_decode_u8(&dec,&out->file_len);
        if(retVal == false) return false;
        
        retVal = mc_decode_bytes(&dec,out->file,out->file_len);
        if(retVal == false) return false;
    }

    if(mask & MEDM_LINE)
    {
        retVal = mc_decode_u32(&dec,&out->line);
        if(retVal == false) return false;
    }

    if(mask & MEDM_SEVERITY)
    {
        retVal = mc_decode_u8(&dec,&out->severity);
        if(retVal == false) return false;
    }

    if(mask & MEDM_FUNCTION)
    {
        retVal = mc_decode_u8(&dec,&out->func_len);
        if(retVal == false) return false;

        retVal = mc_decode_bytes(&dec,out->func,out->func_len);
        if(retVal == false) return false;
    }

    if(mask & MEDM_MESSAGE)
    {
        retVal = mc_decode_u16(&dec,&out->msg_len);
        if(retVal == false) return false;

        retVal = mc_decode_bytes(&dec,out->msg,out->msg_len);
        if(retVal == false) return false;
    }
    

    return true;

}
