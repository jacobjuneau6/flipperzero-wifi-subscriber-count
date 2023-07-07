#include <stdio.h>
#include <furi.h>
#include <furi_hal.h>
#include <gui/gui.h>
#include <input/input.h>


int uart_fd;

void uart_init() {
    uart_fd = flipper_open("/dev/ttyS0", O_RDWR); // Update with your actual serial port
    if (uart_fd < 0) {
        printf("Failed to open UART\n");
        return;
    }
    flipper_configure(uart_fd, FLIPPER_UART_MODE, 115200); // Update with your actual baudrate
}

void uart_send_command(const char* command) {
    flipper_write(uart_fd, command, strlen(command));
    char response[256];
    flipper_readline(uart_fd, response, sizeof(response));
    printf("Response: %s\n", response);
}

int main() {
    lf_attach(0);
    uart_init();
    flipper_button_subscribe(BUTTON_CENTER, 0, uart_send_command);

    while (1) {
        lf_spin();
    }
}
