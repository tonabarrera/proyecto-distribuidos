/*
 * Copyright (c) 2014 Cesanta Software Limited
 * All rights reserved
 */

#include "mongoose.h"
#include "PaqueteDatagrama.h"
#include "SocketDatagrama.h"

static sig_atomic_t s_signal_received = 0;
static const char *s_http_port = "8000";
static struct mg_serve_http_opts s_http_server_opts;

static void signal_handler(int sig_num) {
  signal(sig_num, signal_handler);  // Reinstantiate signal handler
  s_signal_received = sig_num;
}

static int is_websocket(const struct mg_connection *nc) {
  return nc->flags & MG_F_IS_WEBSOCKET;
}

static void broadcast(struct mg_connection *nc, const struct mg_str msg) {
  struct mg_connection *c;
  char buf[500];
  char addr[32];
  mg_sock_addr_to_str(&nc->sa, addr, sizeof(addr),
                      MG_SOCK_STRINGIFY_IP | MG_SOCK_STRINGIFY_PORT);

  snprintf(buf, sizeof(buf), "%s %.*s", addr, (int) msg.len, msg.p);
  printf("%s\n", buf); /* Local echo. */
  for (c = mg_next(nc->mgr, NULL); c != NULL; c = mg_next(nc->mgr, c)) {
    mg_send_websocket_frame(c, WEBSOCKET_OP_TEXT, buf, strlen(buf));
  }
}

struct Mensaje {
    char numTel[11];
    char rfc[14];
    short int partido;
};

char partido[10][10] = {"PAN", "PRI", "PRD", "VERDE", "PT", "MC", "NA", "MORENA", "ES", "BRONCO"};

static void ev_handler(struct mg_connection *nc, int ev, void *ev_data) {
  switch (ev) {
    case MG_EV_WEBSOCKET_HANDSHAKE_DONE: {
      /* New websocket connection. Tell everybody. */
      struct mg_str d = {(char *) "HOLA", 4};
      printf("%s\n", "SE CONECTO");
      break;
    }
    case MG_EV_WEBSOCKET_FRAME: {
      struct websocket_message *wm = (struct websocket_message *) ev_data;
      /* New websocket message. Tell everybody. */
      struct mg_str d = {(char *) wm->data, wm->size};

      char tele[11] = {0};
      memcpy(tele, wm->data, 10);
      printf("EL TELEFONO A BUSCAR: %s\n", tele);
      SocketDatagrama socketDatagrama(7001);
      PaqueteDatagrama paquete(tele, 11, "10.100.79.185", 7001);
      PaqueteDatagrama recibe(sizeof(struct Mensaje));
      socketDatagrama.envia(paquete);
      printf("%s\n", "Esperando...");
      printf("Recibio %d bytes\n", socketDatagrama.recibe(recibe));
      struct Mensaje *votante = (struct Mensaje *)recibe.obtieneDatos();
      printf("La respuesta de telefono fue: %s\n", votante->numTel);
      printf("La respuesta de rfc fue: %s\n", votante->rfc);
      printf("La respuesta de partido fue %d\n", votante->partido);
      if (votante->partido != -1) {
        d = {votante->rfc, 13};
      broadcast(nc, d);
      d = {votante->numTel, 10};
      broadcast(nc, d);
      d = {partido[votante->partido], 10};
      broadcast(nc, d);
      } else {
        d = {"NO", 2};
        broadcast(nc, d);
      }
      printf("%s\n", "-----------");
      break;
    }
    case MG_EV_HTTP_REQUEST: {
      mg_serve_http(nc, (struct http_message *) ev_data, s_http_server_opts);
      break;
    }
    case MG_EV_CLOSE: {
      /* Disconnect. Tell everybody. */
      if (is_websocket(nc)) {
        broadcast(nc, mg_mk_str("-- left"));
      }
      break;
    }
  }
}

int main(void) {
  struct mg_mgr mgr;
  struct mg_connection *nc;

  signal(SIGTERM, signal_handler);
  signal(SIGINT, signal_handler);
  setvbuf(stdout, NULL, _IOLBF, 0);
  setvbuf(stderr, NULL, _IOLBF, 0);

  mg_mgr_init(&mgr, NULL);

  nc = mg_bind(&mgr, s_http_port, ev_handler);
  mg_set_protocol_http_websocket(nc);
  s_http_server_opts.document_root = ".";  // Serve current directory
  s_http_server_opts.enable_directory_listing = "yes";

  printf("Started on port %s\n", s_http_port);
  while (s_signal_received == 0) {
    mg_mgr_poll(&mgr, 200);
  }
  mg_mgr_free(&mgr);

  return 0;
}
