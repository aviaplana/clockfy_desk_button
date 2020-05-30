#ifndef UNIT_TEST
#include "ApiDS.h"

ApiDS::ApiDS() {

}

String ApiDS::generateGetRequest(String* endpoint) {
    String request = F("GET ");

    request += generateCommonRequest(endpoint);
    request += F("\r\n");

    return request;
}

String ApiDS::generateCommonRequest(String* endpoint) {
    String request = endpoint_prefix;
    request += *endpoint;
    request += F(" HTTP/1.1\r\nX-Api-Key: ");
    request += API_KEY;
    request += F("\r\n");
    request += F("content-type: application/json\r\nHost: ");
    request += host;
    request += F("\r\nConnection: close\r\n");

    return request;
}

String ApiDS::generatePostRequest(String* endpoint, String* request_body) {
    String request = F("POST ");

    request += generateCommonRequest(endpoint);
    request += "Content-Length: ";
    request += (*request_body).length();
    request += F("\r\n");
    request += *request_body;
    request += F("\r\n\r\n");

    return request;
}

int ApiDS::readResponseHeaders(WiFiClientSecure* client) {
    #ifdef DEBUG_API
        Serial.println(F("Response headers:"));
    #endif
    char* response_code = (char*) malloc(10);

    client->find("HTTP");
    client->find(" ");
    size_t size = client->readBytesUntil(' ', response_code, 10);
    response_code[size] = '\0';
    int code = atoi(response_code);
    free(response_code);

    #ifdef DEBUG_API
        Serial.printf("%d ", code);
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

    return code;
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

        String request = generateGetRequest(&endpoint);

        #ifdef DEBUG_API
            Serial.print(F("Sending request:\n"));
            Serial.print(request);
            Serial.print(F("\n"));
        #endif

        client.print(request);

        int response_code = readResponseHeaders(&client);
        
        if (response_code == 200) {
            client.find("[{");
            uint8_t counter = 0;
            struct Project** projects = (struct Project**) malloc(sizeof(struct Project*) * 20);

            while (true) {
                projects[counter] = parseResponse((Stream*) &client);
                
                if (projects[counter] == 0) {
                    break;
                }

                counter++;     
            }
        
            return projects;
        } else {
            Serial.print(F("Request failed. Response code: "));
            Serial.println(response_code);
        }
    }

    return 0;
}

Project* ApiDS::parseResponse(Stream* stream) {
    char* buffer = (char*) malloc(100);
    Project* project = (Project*) malloc(sizeof(Project));

    char* tag = (char*) malloc(30);
    strcpy(tag, "\"id\":\"");
    int size = processResponse(tag, buffer, stream);

    if (size < 1) return 0;
    strncpy(project->id, buffer, size);
    project->id[size] = '\0';

    strcpy(tag, "\"name\":\"");
    size = processResponse(tag, buffer, stream);

    if (size < 1) return 0;
    strncpy(project->name, buffer, size);
    project->name[size] = '\0';

    strcpy(tag, "\"color\":\"");
    size = processResponse(tag, buffer, stream);

    if (size < 1) return 0;
    char* color_hex = (char*) malloc(10);
    strncpy(color_hex, buffer, size);
    color_hex[size] = '\0';

    project->color = hexToColor(color_hex);
    free(color_hex);

    #ifdef DEBUG_API
        Serial.print(F("\n"));
    #endif

    free(tag);
    free(buffer);

    return project;
}

Color ApiDS::hexToColor(char* hex) {
    Color color;

    char* part = (char*) malloc(2);
    strncpy(part, hex + 1, 2);
    color.red = (byte) strtol(part, NULL, 16);
    
    strncpy(part, hex + 3, 2);
    color.green = (byte) strtol(part, NULL, 16);

    strncpy(part, hex + 5, 2);
    color.blue = (byte) strtol(part, NULL, 16);

    return color;

}

unsigned int ApiDS::processResponse(char* tag, char* buffer, Stream* stream) {
    if (!stream->find(tag)) return -1;
    size_t size = stream->readBytesUntil('"', buffer, 30);

    #ifdef DEBUG_API
        printResponsePart(tag, buffer, size);
    #endif

    return size;
}

void ApiDS::printResponsePart(char* tag, char* buffer, size_t long_bytes) {
    if (long_bytes == 0) return;

    Serial.printf("%s", tag);

    for (unsigned int i = 0; i < long_bytes; i++) {
        Serial.print(buffer[i]);
    }

    Serial.print(F("\", "));
}

BearSSL::WiFiClientSecure ApiDS::getConnectedClient() {
    #ifdef DEBUG_API
        Serial.print(F("Connecting to "));
        Serial.print(host);
        Serial.print(F(":"));
        Serial.print(port);
        Serial.print(F("\n"));
    #endif

    WiFiClientSecure httpsClient;
    httpsClient.setFingerprint(fingerprint);
    httpsClient.setTimeout(timeout); // 15 Seconds
    delay(100);

    unsigned int retry_counter = 0; //retry counter
    httpsClient.connect(host, port);
    
    while (!httpsClient.connect(host, port) &&
            (retry_counter < max_retry)) {
        retry_counter++;
        delay(100);
    }

    #ifdef DEBUG_API
        if(retry_counter == max_retry) {
            Serial.print(F("Connection failed! Error code: "));
            Serial.print(httpsClient.getLastSSLError());
            Serial.print(F("\n"));
        } else {
            Serial.println(F("Connected!"));
        }
    #endif

    return httpsClient;
}

char* ApiDS::startTimer(char* project_id, char* start_time) {
    WiFiClientSecure client = getConnectedClient();
    
    if (client.connected()) {
        Serial.println(F("Starting timer..."));
        String endpoint = F("/workspaces/");
        endpoint += API_WORKSPACE_ID;
        endpoint += F("/time-entries");
        String request_body = F("{\"start\":"); 
        request_body += start_time;
        request_body += F("\"projectId\":");
        request_body += project_id;
        request_body += F("}");

        String request = generatePostRequest(&endpoint, &request_body);

        #ifdef DEBUG_API
            Serial.print(F("Sending request:\n"));
            Serial.print(request);
            Serial.print(F("\n"));
        #endif

        client.print(request);
        int response_code = readResponseHeaders(&client);

        if (response_code == 201) {
            char* buffer = (char*) malloc(100);
            char* tag = (char*) malloc(30);
            strcpy(tag, "\"id\":\"");
            int size = processResponse(tag, buffer,  (Stream*) &client);

            char* id = (char*) malloc(30);
            strncpy(id, buffer, size);
            id[size] = '\0';
            
            free(buffer);
            free(tag);

            return id;
        } else {
            Serial.print(F("Request failed. Response code: "));
            Serial.println(response_code);
        }

    }

    return "";
}

#endif