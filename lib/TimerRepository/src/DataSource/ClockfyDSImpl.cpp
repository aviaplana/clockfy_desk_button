#ifndef UNIT_TEST
#include "Clockfy/ClockfyDSImpl.h"
#include "Clockfy/clockfy_certificate.h"

ClockfyDSImpl::ClockfyDSImpl() {

}

String ClockfyDSImpl::generateGetRequest(String* endpoint) {
    String request = F("GET ");

    request += generateCommonRequest(endpoint);
    request += F("\r\n");

    return request;
}

String ClockfyDSImpl::generateCommonRequest(String* endpoint) {
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

String ClockfyDSImpl::generatePostRequest(String* endpoint, String* request_body) {
    String request = F("POST ");

    request += generateCommonRequest(endpoint);
    request += generateBodyRequest(request_body);

    return request;
}

String ClockfyDSImpl::generatePatchRequest(String* endpoint, String* request_body) {
    String request = F("PATCH ");

    request += generateCommonRequest(endpoint);
    request += generateBodyRequest(request_body);

    return request;
}

Project** ClockfyDSImpl::getProjects(char* workspace_id) {
    WiFiClientSecure client = getConnectedClient();
    if (client.connected()) {
        Serial.println(F("Getting projects.."));

        String endpoint = F("/workspaces/");
        endpoint += workspace_id;
        endpoint += F("/projects");

        String request = generateGetRequest(&endpoint);

        #ifdef DEBUG_API
            Serial.print(F("Sending request:\n"));
            Serial.print(request);
            Serial.print(F("\n"));
        #endif

        client.print(request);

        int response_code = readResponseHeaders(&client);
        
        if (response_code == OK) {
            client.find("[{");
            uint8_t counter = 0;
            struct Project** projects = (struct Project**) malloc(sizeof(struct Project*) * 20);

            while (true) {
                projects[counter] = parseResponse((Stream*) &client);
                
                if (projects[counter] == 0) {
                    break;
                }

                #ifdef DEBUG_API
                    printProject(projects[counter]);
                #endif

                counter++;     
            }
        
            return projects;
        } else {
            printErrorMessage(response_code);
        }
    }

    return 0;
}

UserData* ClockfyDSImpl::getUserData() {
    WiFiClientSecure client = getConnectedClient();
    if (client.connected()) {
        Serial.println(F("Getting user data.."));

        String endpoint = F("/user");
        String request = generateGetRequest(&endpoint);

        #ifdef DEBUG_API
            Serial.print(F("Sending request:\n"));
            Serial.print(request);
            Serial.print(F("\n"));
        #endif

        client.print(request);

        int response_code = readResponseHeaders(&client);
        
        if (response_code == OK) {
            UserData* user_data = (UserData*) malloc(sizeof(UserData));

            client.find("{");

            char* buffer = (char*) malloc(100);
            char* tag = (char*) malloc(30);
            strcpy(tag, "\"userId\":\"");
            int size = processResponse(tag, buffer,  (Stream*) &client);

            if (size < 1) return 0;
            strncpy(user_data->user_id, buffer, size);
            user_data->user_id[size] = '\0';


            strcpy(tag, "\"activeWorkspace\":\"");
            size = processResponse(tag, buffer,  (Stream*) &client);

            if (size < 1) return 0;
            strncpy(user_data->workspace_id, buffer, size);
            user_data->workspace_id[size] = '\0';
        
            return user_data;
        } else {
            printErrorMessage(response_code);
        }
    }

    return 0;
}

void ClockfyDSImpl::printProject(Project* project) {
    Serial.print(project->id);
    Serial.print("\t");
    Serial.print(project->name);
    Serial.print(F("\tR:"));
    Serial.print(project->color.red);
    Serial.print(F("\tG:"));
    Serial.print(project->color.red);
    Serial.print(F("\tB:"));
    Serial.println(project->color.red);
}

Project* ClockfyDSImpl::parseResponse(Stream* stream) {
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

Color ClockfyDSImpl::hexToColor(char* hex) {
    Color color {0, 0, 0};

    char* part = (char*) malloc(2);
    strncpy(part, hex + 1, 2);
    color.red = (byte) strtol(part, NULL, 16);
    
    strncpy(part, hex + 3, 2);
    color.green = (byte) strtol(part, NULL, 16);

    strncpy(part, hex + 5, 2);
    color.blue = (byte) strtol(part, NULL, 16);

    return color;
}

BearSSL::WiFiClientSecure ClockfyDSImpl::getConnectedClient() {
    #ifdef DEBUG_API
        Serial.print(F("Connecting to "));
        Serial.print(host);
        Serial.print(F(":"));
        Serial.print(port);
        Serial.print(F("\n"));
    #endif

    WiFiClientSecure httpsClient;
    
    // Load root certificate in DER format into WiFiClientSecure object
    bool res = httpsClient.setCACert(root_ca_certificate, CLOCKFY_CERTIFICATE_LENGTH);

    if (!res) {
        Serial.println("Failed to load root CA certificate!");

        while (true) {
            yield();
        }
    }

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

bool ClockfyDSImpl::startTimer(char* project_id, char* workspace_id, char* start_time) {
    WiFiClientSecure client = getConnectedClient();
    
    if (client.connected()) {
        Serial.println(F("Starting timer..."));
        String endpoint = F("/workspaces/");
        endpoint += workspace_id;
        endpoint += F("/time-entries");
        String request_body = F("{\"start\":\""); 
        request_body += start_time;
        request_body += F("\",\"projectId\":\"");
        request_body += project_id;
        request_body += F("\"}");

        String request = generatePostRequest(&endpoint, &request_body);

        #ifdef DEBUG_API
            Serial.print(F("Sending request:\n"));
            Serial.print(request);
            Serial.print(F("\n"));
        #endif

        client.print(request);
        int response_code = readResponseHeaders(&client);

        if (response_code == CREATED) {
            char* buffer = (char*) malloc(100);
            char* tag = (char*) malloc(30);
            strcpy(tag, "\"id\":\"");
            int size = processResponse(tag, buffer,  (Stream*) &client);

            char* id = (char*) malloc(30);
            strncpy(id, buffer, size);
            id[size] = '\0';
            
            #ifdef DEBUG_API
                Serial.print(F("Timer id: "));
                Serial.print(id);
                Serial.print(F("\n"));
            #endif

            free(buffer);
            free(tag);

            return true;
        } else {
            printErrorMessage(response_code);
        }
    }
    return false;
}

bool ClockfyDSImpl::stopTimer(char* stop_time, char* user_id, char* workspace_id) {
    WiFiClientSecure client = getConnectedClient();
    
    if (client.connected()) {
        Serial.println(F("Stopping timer..."));
        String endpoint = F("/workspaces/");
        endpoint += workspace_id;
        endpoint += F("/user/");
        endpoint += user_id;
        endpoint += F("/time-entries");
        String request_body = F("{\"end\":\""); 
        request_body += stop_time;
        request_body += F("\"}");

        String request = generatePatchRequest(&endpoint, &request_body);

        #ifdef DEBUG_API
            Serial.print(F("Sending request:\n"));
            Serial.print(request);
            Serial.print(F("\n"));
        #endif

        client.print(request);
        int response_code = readResponseHeaders(&client);

        if (response_code == OK) {
            String response = getResponseBody((Stream*) &client);
            
            #ifdef DEBUG_API
                Serial.print(F("Success. Response:\n"));
                Serial.print(response);
                Serial.print(F("\n"));
            #endif
            
            return true;
        } else {
            printErrorMessage(response_code);
        }
    }
    return false;
}

#endif