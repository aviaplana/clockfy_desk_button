#include "ApiDS.h"

ApiDS::ApiDS() {

}

String ApiDS::getRequest(String* endpoint) {
    String headers;
    return getRequest(endpoint, &headers);
}

String ApiDS::getRequest(String* endpoint, String* headers) {
    String request = F("GET ");
    request += endpoint_prefix;
    request += *endpoint;
    request += F(" HTTP/1.1\r\nX-Api-Key: ");
    request += API_KEY;
    request += F("\r\n");
    request += *headers;
    request += F("content-type: application/json\r\nHost: ");
    request += host;
    request += F("\r\nConnection: close\r\n\r\n");

    return request;
}

void ApiDS::readResponseHeaders(WiFiClientSecure* client) {
    #ifdef DEBUG_API
        Serial.println(F("Response headers:"));
    #endif

    while (client->connected()) {
        String line = client->readStringUntil('\n');

        #ifdef DEBUG_API
            Serial.println(line);
        #endif

        if (line == "\r") {
            break;
        }
    }
}

String ApiDS::getResponseBody(WiFiClientSecure* client) {
    String response;

    while(client->available()){        
        response += client->readStringUntil('\n'); 
    }      

    #ifdef DEBUG_API
        Serial.println(response); 
    #endif

    return response;
}

Project** ApiDS::getProjects() {
    WiFiClientSecure client = getConnectedClient();
    if (client.connected()) {
        Serial.println(F("Getting projects.."));

        String endpoint = F("/workspaces/");
        endpoint += API_WORKSPACE_ID;
        endpoint += F("/projects");

        String request = getRequest(&endpoint);

        #ifdef DEBUG_API
            Serial.print(F("Sending request:\n"));
            Serial.print(request);
            Serial.print(F("\n"));
        #endif

        client.print(request);

        readResponseHeaders(&client);
        String response = getResponseBody(&client);
        
        return 0;
    }
}

BearSSL::WiFiClientSecure ApiDS::getConnectedClient() {
    #ifdef DEBUG_API
        Serial.printf("Connecting to %s:%d\n", host, port);
    #endif

    WiFiClientSecure httpsClient;
    httpsClient.setFingerprint(fingerprint);
    httpsClient.setTimeout(timeout); // 15 Seconds
    delay(100);

    unsigned int retry_counter = 0; //retry counter
    int code = httpsClient.connect(host, port);
    
    while (!httpsClient.connect(host, port) &&
            (retry_counter < max_retry)) {
        retry_counter++;
        delay(100);
    }

    #ifdef DEBUG_API
        if(retry_counter == max_retry) {
            Serial.printf("Connection failed! Error code: %d\n", httpsClient.getLastSSLError());
        } else {
            Serial.println(F("Connected!"));
        }
    #endif

    return httpsClient;
}
