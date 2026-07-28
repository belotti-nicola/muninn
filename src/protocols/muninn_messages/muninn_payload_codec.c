#include <internal/protocols/muninn_messages/muninn_payload_codec.h>

#include <internal/protocols/muninn_messages/muninn_message.h>
#include <internal/protocols/muninn_messages/muninn_message_mask.h>
#include <internal/protocols/muninn_messages/muninn_codec_utility.h>

bool muninn_payload_encode(muninn_payload *muninn_payload, muninn_message_mask mask, uint8_t *out, size_t *out_size)
{
    if(muninn_payload == NULL || out == NULL || out_size == NULL) return false;
    
    if(out_size == 0) return false;

    mcu_encoder_t enc;
    mcu_encoder_setup(&enc,out,*out_size);

    bool retVal = true;

    if(true)
    {
        retVal = mcu_encode_u32(&enc,mask);
        if(retVal == false) return false;
    }

    if(mask & MEDM_PID )
    {
        retVal = mcu_encode_u32(&enc,muninn_payload->pid);
        if(retVal == false) return false;
    }

    if(mask & MEDM_THREAD)
    {
        retVal = mcu_encode_u64(&enc,muninn_payload->thread_id);
        if(retVal == false) return false;
    }

    if(mask & MEDM_TIMESTAMP)
    {
        retVal = mcu_encode_u64(&enc,muninn_payload->timestamp);
        if(retVal == false) return false;
    }

    if(mask & MEDM_FILE)
    {
        retVal = mcu_encode_u8(&enc,muninn_payload->file_len);
        if(retVal == false) return false;
        
        retVal = mcu_encode_bytes(&enc,muninn_payload->file,muninn_payload->file_len);
        if(retVal == false) return false;
    }

    if(mask & MEDM_LINE)
    {
        retVal = mcu_encode_u32(&enc,muninn_payload->line);
        if(retVal == false) return false;
    }

    if(mask & MEDM_SEVERITY)
    {
        retVal = mcu_encode_u8(&enc,muninn_payload->severity);
        if(retVal == false) return false;
    }

    if(mask & MEDM_FUNCTION)
    {
        retVal = mcu_encode_u8(&enc,muninn_payload->func_len);
        if(retVal == false) return false;

        retVal = mcu_encode_bytes(&enc,muninn_payload->func,muninn_payload->func_len);
        if(retVal == false) return false;
    }

    if(mask & MEDM_MESSAGE)
    {
        retVal = mcu_encode_u16(&enc,muninn_payload->msg_len);
        if(retVal == false) return false;

        retVal = mcu_encode_bytes(&enc,muninn_payload->msg,muninn_payload->msg_len);
        if(retVal == false) return false;
    }

    *out_size = enc.current_size;

    return true;
}

bool muninn_payload_decode(const uint8_t *in, size_t in_size, muninn_payload *muninn_payload)
{
    if( muninn_payload == NULL || in == NULL || in_size == 0 ) return false;

    bool retVal = true;
    
    mcu_decoder_t dec;
    mcu_decoder_setup(&dec,in,in_size);

    muninn_message_mask mask;
    mcu_decode_u32(&dec,&mask);

    if(mask & MEDM_PID )
    {
        retVal = mcu_decode_u32(&dec,&muninn_payload->pid);
        if(retVal == false) return false;
    }

    if(mask & MEDM_THREAD)
    {
        retVal = mcu_decode_u64(&dec,&muninn_payload->thread_id);
        if(retVal == false) return false;
    }

    if(mask & MEDM_TIMESTAMP)
    {
        retVal = mcu_decode_u64(&dec,&muninn_payload->timestamp);
        if(retVal == false) return false;
    }

    if(mask & MEDM_FILE)
    {
        retVal = mcu_decode_u8(&dec,&muninn_payload->file_len);
        if(retVal == false) return false;
        
        retVal = mcu_decode_bytes(&dec,muninn_payload->file,muninn_payload->file_len);
        if(retVal == false) return false;
    }

    if(mask & MEDM_LINE)
    {
        retVal = mcu_decode_u32(&dec,&muninn_payload->line);
        if(retVal == false) return false;
    }

    if(mask & MEDM_SEVERITY)
    {
        retVal = mcu_decode_u8(&dec,&muninn_payload->severity);
        if(retVal == false) return false;
    }

    if(mask & MEDM_FUNCTION)
    {
        retVal = mcu_decode_u8(&dec,&muninn_payload->func_len);
        if(retVal == false) return false;

        retVal = mcu_decode_bytes(&dec,muninn_payload->func,muninn_payload->func_len);
        if(retVal == false) return false;
    }

    if(mask & MEDM_MESSAGE)
    {
        retVal = mcu_decode_u16(&dec,&muninn_payload->msg_len);
        if(retVal == false) return false;

        retVal = mcu_decode_bytes(&dec,muninn_payload->msg,muninn_payload->msg_len);
        if(retVal == false) return false;
    }

    return true;
}