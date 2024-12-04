#include <stdio.h>
#include <string.h>

#define REPORT_SIZE 6

void identify_key_changes(unsigned char previous_report[], unsigned char current_report[], unsigned char pressed_keys[], unsigned char released_keys[], int *pressed_count, int *released_count)
{
    *pressed_count = 0;
    *released_count = 0;

    // Identify pressed keys
    for (int i = 0; i < REPORT_SIZE; i++) {
        if (current_report[i] != 0) {
            int found = 0;
            for (int j = 0; j < REPORT_SIZE; j++) {
                if (current_report[i] == previous_report[j]) {
                    found = 1;
                    break;
                }
            }
            if (!found) {
                pressed_keys[(*pressed_count)++] = current_report[i];
            }
        }
    }

    // Identify released keys
    for (int i = 0; i < REPORT_SIZE; i++) {
        if (previous_report[i] != 0) {
            int found = 0;
            for (int j = 0; j < REPORT_SIZE; j++) {
                if (previous_report[i] == current_report[j]) {
                    found = 1;
                    break;
                }
            }
            if (!found) {
                released_keys[(*released_count)++] = previous_report[i];
            }
        }
    }
}

int main()
{
    unsigned char previous_report[REPORT_SIZE] = { 4, 0, 0, 0, 0, 0 }; // 'a' key pressed
    unsigned char current_report[REPORT_SIZE] = { 0, 0, 0, 0, 0, 0 };  // No key pressed

    unsigned char pressed_keys[REPORT_SIZE];
    unsigned char released_keys[REPORT_SIZE];
    int pressed_count, released_count;

    identify_key_changes(previous_report, current_report, pressed_keys, released_keys, &pressed_count, &released_count);

    printf("Pressed keys: ");
    for (int i = 0; i < pressed_count; i++) {
        printf("%d ", pressed_keys[i]);
    }
    printf("\n");

    printf("Released keys: ");
    for (int i = 0; i < released_count; i++) {
        printf("%d ", released_keys[i]);
    }
    printf("\n");

    return 0;
}
