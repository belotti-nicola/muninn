#include <internal/protocols/muninn_encoded_messages/muninn_encoded_message_codec.h>

#include <internal/muninn_message.h>
#include <internal/protocols/muninn_encoded_messages/muninn_encoded_message_mask.h>
#include <internal/protocols/muninn_encoded_messages/muninn_encoded_message_codec_utility.h>


bool muninn_messages_encode(muninn_message_t *mm, muninn_message_mask mask, uint8_t *out, size_t out_size, size_t *written_bytes)
{
    if(mm == NULL || out == NULL || written_bytes == NULL ) return false;

    mcu_encoder_t enc;
    mcu_encoder_setup(&enc,out,out_size);

    bool retVal;

    if( true )
    {
        retVal = mcu_encode_u32(&enc,mask);
        if(retVal == false) return false;
    }

    if(mask & MEDM_PID )
    {
        retVal = mcu_encode_u32(&enc,mm->pid);
        if(retVal == false) return false;
    }

    if(mask & MEDM_THREAD)
    {
        retVal = mcu_encode_u64(&enc,mm->thread_id);
        if(retVal == false) return false;
    }

    if(mask & MEDM_TIMESTAMP)
    {
        retVal = mcu_encode_u64(&enc,mm->timestamp);
        if(retVal == false) return false;
    }

    if(mask & MEDM_FILE)
    {
        retVal = mcu_encode_u8(&enc,mm->file_len);
        if(retVal == false) return false;
        
        retVal = mcu_encode_bytes(&enc,mm->file,mm->file_len);
        if(retVal == false) return false;
    }

    if(mask & MEDM_LINE)
    {
        retVal = mcu_encode_u32(&enc,mm->line);
        if(retVal == false) return false;
    }

    if(mask & MEDM_SEVERITY)
    {
        retVal = mcu_encode_u8(&enc,mm->severity);
        if(retVal == false) return false;
    }

    if(mask & MEDM_FUNCTION)
    {
        retVal = mcu_encode_u8(&enc,mm->func_len);
        if(retVal == false) return false;

        retVal = mcu_encode_bytes(&enc,mm->func,mm->func_len);
        if(retVal == false) return false;
    }

    if(mask & MEDM_MESSAGE)
    {
        retVal = mcu_encode_u16(&enc,mm->msg_len);
        if(retVal == false) return false;

        retVal = mcu_encode_bytes(&enc,mm->msg,mm->msg_len);
        if(retVal == false) return false;
    }  

    *written_bytes = enc.current_size;

    return true;
}

bool muninn_messages_decode(const uint8_t *in, size_t in_size, muninn_message_t *mm)
{
    if(in == NULL || mm == NULL || in_size == 0) return false;

    mcu_decoder_t dec;
    mcu_decoder_setup(&dec,in,in_size);

    bool retVal;

    muninn_message_mask mask;

    if ( true )
    {
        retVal = mcu_decode_u32(&dec,&mask);
        if(retVal == false) return false;
    }

    if(mask & MEDM_PID )
    {
        retVal = mcu_decode_u32(&dec,&mm->pid);
        if(retVal == false) return false;
    }

    if(mask & MEDM_THREAD)
    {
        retVal = mcu_decode_u64(&dec,&mm->thread_id);
        if(retVal == false) return false;
    }

    if(mask & MEDM_TIMESTAMP)
    {
        retVal = mcu_decode_u64(&dec,&mm->timestamp);
        if(retVal == false) return false;
    }

    if(mask & MEDM_FILE)
    {
        retVal = mcu_decode_u8(&dec,&mm->file_len);
        if(retVal == false) return false;
        
        retVal = mcu_decode_bytes(&dec,mm->file,mm->file_len);
        if(retVal == false) return false;
    }

    if(mask & MEDM_LINE)
    {
        retVal = mcu_decode_u32(&dec,&mm->line);
        if(retVal == false) return false;
    }

    if(mask & MEDM_SEVERITY)
    {
        retVal = mcu_decode_u8(&dec,&mm->severity);
        if(retVal == false) return false;
    }

    if(mask & MEDM_FUNCTION)
    {
        retVal = mcu_decode_u8(&dec,&mm->func_len);
        if(retVal == false) return false;

        retVal = mcu_decode_bytes(&dec,mm->func,mm->func_len);
        if(retVal == false) return false;
    }

    if(mask & MEDM_MESSAGE)
    {
        retVal = mcu_decode_u16(&dec,&mm->msg_len);
        if(retVal == false) return false;

        retVal = mcu_decode_bytes(&dec,mm->msg,mm->msg_len);
        if(retVal == false) return false;
    }

    return true;
}