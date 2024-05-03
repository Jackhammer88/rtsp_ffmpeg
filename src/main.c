#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <getopt.h>
#include "rtsp_listener.h"

int main(int argc, char** argv) {
    const int urlLength = 250;
    char url[urlLength];
    int quality = -1;

    memset(url, 0, sizeof(url));

    struct option long_options[] = {
            {"url", required_argument, NULL, 'u'},
            {"quality", required_argument, NULL, 'q'},
            {"help", no_argument, NULL, 'h'},
            {NULL, 0, NULL, 0}
    };

    int opt;
    int option_index = 0;

    while ((opt = getopt_long(argc, argv, "u:q:h", long_options, &option_index)) != -1) {
        switch (opt) {
            case 'u':
                strncpy(url, optarg, urlLength - 1);
                url[urlLength - 1] = '\0';
                break;
            case 'q':
                quality = atoi(optarg);
                if (quality < 0 || quality > 100) {
                    fprintf(stderr, "Качество должно быть между 0 и 100\n");
                    exit(EXIT_FAILURE);
                }
                break;
            case 'h':
                fprintf(stdout, "Описание программы\n");
                fprintf(stdout, "Это приложение предназначено для обработки RTSP потоков.\n");
                fprintf(stdout, "Приложение сохраняет кадр каждую секунду в формате jpeg в директории frames.\n");
                fprintf(stdout, "Вы можете указать URL потока и качество JPEG.\n\n");
                fprintf(stdout, "Использование: %s [-u|--url <url>] [-q|--quality <quality>] [-h|--help]\n", argv[0]);
                exit(EXIT_SUCCESS);
            default:
                fprintf(stderr, "Использование: %s [-u|--url <url>] [-q|--quality <quality>] [-h|--help]\n", argv[0]);
                exit(EXIT_FAILURE);
        }
    }

    if (quality == -1) quality = 75;

    printf("URL: %s\n", strlen(url) > 0 ? url : "не указан");
    printf("Качество: %d\n", quality);

    if (strlen(url) == 0) {
        printf("Введите URL:\n");
        if (!fgets(url, urlLength, stdin)) {
            return -1;
        }
        url[strcspn(url, "\n")] = 0;
        if (!strlen(url)) {
            fprintf(stderr, "URL не был введен.\n");
            return -1;
        }
    }

    startProcessing(url, quality);
    return 0;
}
