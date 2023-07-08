#include "flipper.h"
#include <furi.h>
#include <gui/gui.h>
#include <input/input.h>
#include <stdlib.h>

typedef enum {
    EventTypeTick,
    EventTypeKey,
} EventType;

typedef struct {
    EventType type;
    InputEvent input;
} PluginEvent;

typedef struct {
    int x;
    int y;
} PluginState;

static void input_callback(InputEvent* input_event, FuriMessageQueue* event_queue) {
    furi_assert(event_queue);

    PluginEvent event = {.type = EventTypeKey, .input = *input_event};
    furi_message_queue_put(event_queue, &event, FuriWaitForever);
}

int main() {
    FuriMessageQueue* event_queue = furi_message_queue_alloc(8, sizeof(PluginEvent));

    uart_configure(115200);
    while (1) {
        PluginEvent event;
        FuriStatus event_status = furi_message_queue_get(event_queue, &event, 100);

        if(event_status == FuriStatusOk) {
            if(event.type == EventTypeKey) {
                if(event.input.type == InputTypePress) {
                    if(event.input.key == InputKeyOk) {
                        uart_write("AT", 2);
                        vTaskDelay(30000);
                        uint8_t buffer[64];
                        size_t bytes_read = uart_read(buffer, sizeof(buffer));
                        if (bytes_read > 0) {
                            buffer[bytes_read] = '\0';
                            lcd_printf("%s", buffer);
                        }
                        break;
                    } else if (event.input.key == InputKeyBack) {
                        break;
                    }
                }
            }
        }
    }

    furi_message_queue_free(event_queue);
    return 0;
}
