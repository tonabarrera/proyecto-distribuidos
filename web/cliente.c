// Copyright (c) 2015 Cesanta Software Limited
// All rights reserved
#include "mongoose.h"


static const char *s_http_port = "8000";
static struct mg_serve_http_opts s_http_server_opts;


static void ev_handler(struct mg_connection *nc, int ev, void *p) {
    char RFC[13] = {0};
    if (ev == MG_EV_HTTP_REQUEST) {
        struct http_message *mensaje =(struct http_message *) p;
        printf("LA url%s TERMINA\n", mensaje->uri);
        if (mensaje->uri.p[1] == 'r') {
            printf("%s\n", "CONSULTA");
            memcpy(RFC, mensaje->uri.p+14, 13);
            printf("%s\n", RFC);
            //mg_serve_http(nc, mensaje, s_http_server_opts);
            mg_printf(nc, "HTTP/1.1 200 OK\r\n"
            "Cache: no-cache\r\n"
            "Content-Type: application/json\r\n"
            "Content-Length: %d\r\n"
            "\r\n");
            //mg_serve_http(nc, mensaje, s_http_server_opts);
        } else
            mg_serve_http(nc, mensaje, s_http_server_opts);
    }
}
int main(void) {
    struct mg_mgr mgr;
    struct mg_connection *nc;
    mg_mgr_init(&mgr, NULL);
    printf("Starting web server on port %s\n", s_http_port);
    nc = mg_bind(&mgr, s_http_port, ev_handler);
    if (nc == NULL) {
        printf("Failed to create listener\n");
        return 1;
    }
    // Set up HTTP server parameters
    mg_set_protocol_http_websocket(nc);
    s_http_server_opts.document_root = "."; // Serve current directory
    s_http_server_opts.enable_directory_listing = "yes";
    for (;;) {
        mg_mgr_poll(&mgr, 1000);
    }
    mg_mgr_free(&mgr);
    return 0;
}