#include "ring_buffer.h"
#include <stdio.h>

#include <inttypes.h> // <-- Fondamentale per i tipi fissi (PRIu32, PRIu64)

// Assumiamo che tu abbia appena fatto una "pop" dal Ring Buffer e 
// abbia ricomposto il messaggio dentro questa variabile 'msg'
void print_message(message_t *msg) 
{
    // 1. Calcoliamo la lunghezza ESATTA del solo testo.
    uint32_t payload_len = msg->header.total_size - sizeof(message_header_t);

    printf("--- Pacchetto Ricevuto ---\n");
    
    // PRIu32 sta per "Print Unsigned 32-bit". È una macro che si concatena alla stringa.
    printf("Total Size: %" PRIu32 " bytes\n", msg->header.total_size);
    
    // PRIu64 per il timestamp a 64 bit
    printf("Timestamp : %" PRIu64 "\n", msg->header.timestamp);
    
    // Per un uint8_t, la normale %u o %d va benissimo, 
    // il C lo promuove automaticamente a intero per la printf.
    printf("Severity  : %u\n", msg->header.severity); 
    
    // 2. Il trucco del %.*s che avevi già imparato!
    // Usiamo il payload_len calcolato sopra, castato a int.
    printf("Payload   : %.*s\n", (int)payload_len, msg->payload);
    
    printf("--------------------------\n");
}

void print_rb(ring_buffer rb)
{
    printf("rb start: %zu\n",rb.start);
    printf("rb end  : %zu\n",rb.end);
    printf("rb size : %zu\n",rb.cur_size);
    printf("--------------------------\n");
}

int main()
{
    const char *tests[] = {
    "Arma virumque cano, Troiae qui primus ab oris",
    "Italiam fato profugus Laviniaque venit",
    "litora, multum ille et terris iactatus et alto,",
    "vi superum, saevae memorem Iunonis ob iram,",
    "multa quoque et bello passus, dum conderet urbem",
    "inferretque deos Latio, genus unde Latinum",
    "Albanique patres atque altae moenia Romae."
    };
    size_t n_messages = sizeof(tests) / sizeof(tests[0]);

    ring_buffer rb={0}; rb_setup(&rb);
    
    for (size_t i = 0; i < n_messages; i++) 
    {
        rb_push(&rb, (uint8_t)(i * 10), tests[i]);
        print_rb(rb);
    }

    printf("\n\nPopping...\n\n\n");

    message_t msg = {0};
    for (size_t i = 0; i < n_messages; i++) 
    {
        rb_pop(&rb, &msg);
        print_rb(rb);
        print_message(&msg);
        printf("\n");
    }

    return 0;
}