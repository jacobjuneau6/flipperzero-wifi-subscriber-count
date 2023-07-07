#include <stdio.h>
#include <furi.h>
#include <furi_hal.h>
#include <gui/gui.h>
#include <input/input.h>

int uart_fd;
int selected_baud_rate = 115200; // Default baud rate

void uart_init() {
    uart_fd = flipper_open("/dev/ttyS0", O_RDWR); // Update with your actual serial port
    if (uart_fd < 0) {
        printf("Failed to open UART\n");
        return;
    }
    flipper_configure(uart_fd, FLIPPER_UART_MODE, selected_baud_rate);
}

void uart_send_command(const char* command) {
    flipper_write(uart_fd, command, strlen(command));
    char response[256];
    flipper_readline(uart_fd, response, sizeof(response));
    printf("Response: %s\n", response);
}

void menu_select_baud_rate() {
    printf("Select Baud Rate:\n");
    printf("1. 9600\n");
    printf("2. 115200 (default)\n");
    printf("3. 57600\n");
    printf("4. 38400\n");

    int option;
    printf("Enter option: ");
    scanf("%d", &option);

    switch (option) {
        case 1:
            selected_baud_rate = 9600;
            break;
        case 2:
            selected_baud_rate = 115200;
            break;
        case 3:
            selected_baud_rate = 57600;
            break;
        case 4:
            selected_baud_rate = 38400;
            break;
        default:
            printf("Invalid option\n");
            return;
    }

    flipper_configure(uart_fd, FLIPPER_UART_MODE, selected_baud_rate);
    printf("Baud rate set to %d\n", selected_baud_rate);
}

void on_button_press(button_t button) {
    switch (button) {
        case BUTTON_CENTER:
            uart_send_command("YOUR_UART_COMMAND\r\n"); // Update with your actual UART command
            break;
        case BUTTON_RIGHT:
            menu_select_baud_rate();
            break;
        default:
            break;
    }
}

int main() {
    lf_attach(0);
    uart_init();
    flipper_button_subscribe(BUTTON_CENTER | BUTTON_RIGHT, 0, on_button_press);

    while (1) {
        lf_spin();
    }
}
