idf_component_register(SRCS "main.c" "wifi_con.c" "clock.c" "mqtt_con.c" "wifi_eap.c"
                    INCLUDE_DIRS "."
                    EMBED_TXTFILES  ca.pem client.crt client.key
)
