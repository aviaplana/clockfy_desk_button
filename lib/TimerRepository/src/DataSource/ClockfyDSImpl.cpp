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
    request += F("\r\nConnection: keep-alive\r\n");

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

String ClockfyDSImpl::generateBodyRequest(String* request_body) {
    String request = "Content-Length: ";
    request += (*request_body).length();
    request += F("\r\n\r\n");
    request += *request_body;
    request += F("\r\n\r\n");

    return request;
}

void ClockfyDSImpl::printProject(Project* project) {
    Serial.print(project->id);
    Serial.print("\t");
    Serial.print(project->name);
    Serial.print(F("\tR:"));
    Serial.print(project->color.red);
    Serial.print(F("\tG:"));
    Serial.print(project->color.green);
    Serial.print(F("\tB:"));
    Serial.println(project->color.blue);
}

void ClockfyDSImpl::printResponsePart(char* tag, char* buffer, size_t long_bytes) {
    if (long_bytes == 0) return;

    Serial.printf("%s", tag);

    for (unsigned int i = 0; i < long_bytes; i++) {
        Serial.print(buffer[i]);
    }

    Serial.print(F("\"\n "));
}

void ClockfyDSImpl::printErrorMessage(int response_code) {
    Serial.print(F("Request failed. Response code: "));
    Serial.println(response_code);
}

Project* ClockfyDSImpl::parseResponseToProject() {
    char* buffer = (char*) malloc(100);
    Project* project = (Project*) malloc(sizeof(Project));

    char* tag = (char*) malloc(30);
    strcpy(tag, "\"id\":\"");
    int size = processResponse(tag, buffer);
    
    if (size < 1) return 0;
    strncpy(project->id, buffer, size);
    project->id[size] = '\0';

    strcpy(tag, "\"name\":\"");
    size = processResponse(tag, buffer);

    if (size < 1) return 0;
    strncpy(project->name, buffer, size);
    project->name[size] = '\0';

    strcpy(tag, "\"color\":\"");
    size = processResponse(tag, buffer);

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

bool ClockfyDSImpl::isHttpsClientReadyToUse() {
    if (httpsClient.connected()) {
        return true;
    }

    #ifdef DEBUG_API
        Serial.print(F("Connecting to "));
        Serial.print(host);
        Serial.print(F(":"));
        Serial.print(port);
        Serial.print(F("\n"));
    #endif

    bool res = httpsClient.setCACert(root_ca_certificate, CLOCKFY_CERTIFICATE_LENGTH);

    if (!res) {
        Serial.println("Failed to load root CA certificate!");

        while (true) {
            yield();
        }
    }

    httpsClient.setTimeout(timeout); // 15 Seconds

    unsigned int retry_counter = 0; //retry counter
    httpsClient.connect(host, port);
    
    while (!httpsClient.connect(host, port) &&
            (retry_counter < max_retry)) {
        retry_counter++;
        delay(100);
    }

    if(retry_counter == max_retry) {
        #ifdef DEBUG_API
            Serial.print(F("Connection failed! Error code: "));
            Serial.print(httpsClient.getLastSSLError());
            Serial.print(F("\n"));
        #endif

        return false;
    } else {
        #ifdef DEBUG_API
            Serial.println(F("Connected!"));
        #endif
        return true;
    }

}

int ClockfyDSImpl::readResponseHeaders() {
    #ifdef DEBUG_API
        Serial.println(F("Response headers:"));
    #endif
    char* response_code = (char*) malloc(10);

    httpsClient.setTimeout(15000);
    httpsClient.find("HTTP");
    httpsClient.find(" ");
    size_t size = httpsClient.readBytesUntil(' ', response_code, 10);
    response_code[size] = '\0';
    int code = atoi(response_code);
    free(response_code);

    #ifdef DEBUG_API
        Serial.printf("%d ", code);
    #endif
    
    httpsClient.setTimeout(100);
    while (httpsClient.available()) {
        String line = httpsClient.readStringUntil('\n');

        #ifdef DEBUG_API
            Serial.println(line);
        #endif

        if (line == "\r") {
            break;
        }
    }

    #ifdef DEBUG_API
        Serial.println("End header!");
    #endif

    return code;
}

String ClockfyDSImpl::getResponseBody() {
    String response;

    while(httpsClient.available()){        
        response += httpsClient.readStringUntil('\n'); 
    }      

    #ifdef DEBUG_API
        Serial.println(response); 
    #endif

    return response;
}

unsigned int ClockfyDSImpl::processResponse(char* tag, char* buffer) {
    if (!httpsClient.find(tag)) return -1;
    size_t size = httpsClient.readBytesUntil('"', buffer, 30);

    #ifdef DEBUG_API
        printResponsePart(tag, buffer, size);
    #endif

    return size;
}

void ClockfyDSImpl::flushBuffers() {
    #ifdef DEBUG_API
        Serial.println(F("Flushing buffers..."));
    #endif
    httpsClient.flush();

    while(httpsClient.available()) {

        #ifdef DEBUG_API
            Serial.print(httpsClient.readString());
        #else
            httpsClient.read();
        #endif
        
        delay(10);
    }

    #ifdef DEBUG_API
        Serial.println(F("\nDone"));
    #endif
}

Project** ClockfyDSImpl::getProjects(char* workspace_id) {
    if (isHttpsClientReadyToUse()) {
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

        httpsClient.print(request);

        int response_code = readResponseHeaders();
        
        if (response_code == OK) {
            httpsClient.find("[{");
            uint8_t counter = 0;
            struct Project** projects = (struct Project**) malloc(sizeof(struct Project*) * 20);

            while (true) {
                projects[counter] = parseResponseToProject();
                
                if (projects[counter] == 0) {
                    break;
                }

                #ifdef DEBUG_API
                    printProject(projects[counter]);
                #endif

                counter++;     
            }

            flushBuffers();
            return projects;
        } else {
            flushBuffers();
            printErrorMessage(response_code);
        }
    }

    return 0;
}

UserData* ClockfyDSImpl::getUserData() {
    if (isHttpsClientReadyToUse()) {
        Serial.println(F("Getting user data.."));

        String endpoint = F("/user");
        String request = generateGetRequest(&endpoint);

        #ifdef DEBUG_API
            Serial.print(F("Sending request:\n"));
            Serial.print(request);
            Serial.print(F("\n"));
        #endif

        httpsClient.print(request);

        int response_code = readResponseHeaders();
        
        if (response_code == OK) {
            UserData* user_data = (UserData*) malloc(sizeof(UserData));

            httpsClient.find("{");

            char* buffer = (char*) malloc(100);
            char* tag = (char*) malloc(30);
            strcpy(tag, "\"userId\":\"");
            int size = processResponse(tag, buffer);

            if (size < 1) return 0;
            strncpy(user_data->user_id, buffer, size);
            user_data->user_id[size] = '\0';


            strcpy(tag, "\"activeWorkspace\":\"");
            size = processResponse(tag, buffer);

            if (size < 1) return 0;
            strncpy(user_data->workspace_id, buffer, size);
            user_data->workspace_id[size] = '\0';
        
            #ifdef DEBUG_API
                Serial.print(F("\n"));
            #endif
            
            flushBuffers();
            free(buffer);
            free(tag);
            return user_data;
        } else {
            flushBuffers();
            printErrorMessage(response_code);
        }
    }

    return 0;
}

char* ClockfyDSImpl::getCurrentTimer(char* user_id, char* workspace_id) {    
    if (isHttpsClientReadyToUse()) {
        Serial.println(F("Checking if there's a timer running..."));
        String endpoint = F("/workspaces/");
        endpoint += workspace_id;
        endpoint += F("/user/");
        endpoint += user_id;
        endpoint += F("/time-entries?in-progress=true");
        
        String request = generateGetRequest(&endpoint);

        #ifdef DEBUG_API
            Serial.print(F("Sending request:\n"));
            Serial.print(request);
            Serial.print(F("\n"));
        #endif

        httpsClient.print(request);
        int response_code = readResponseHeaders();

        if (response_code == OK) {         
            char* tag = "\"projectId\":\"";
            char* buffer = (char*) malloc(30);
            int size = processResponse(tag, buffer);
            
            flushBuffers();
            
            if (size > 0) {
                buffer[size] = '\0';

                #ifdef DEBUG_API
                    Serial.print(F("Success. Timer running for project with id:\n"));
                    Serial.print(buffer);
                    Serial.print(F("\n"));
                #endif
                
                return buffer;
            } else {
                #ifdef DEBUG_API
                    Serial.println(F("Success. No timers running.\n"));
                #endif
            }
        } else {
            printErrorMessage(response_code);
        }
    }

    return NULL;
}

bool ClockfyDSImpl::startTimer(char* project_id, char* workspace_id, char* start_time) {   
    if (isHttpsClientReadyToUse()) {
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

        httpsClient.print(request);
        int response_code = readResponseHeaders();

        if (response_code == CREATED) {
            char* buffer = (char*) malloc(100);
            char* tag = (char*) malloc(30);
            strcpy(tag, "\"id\":\"");
            int size = processResponse(tag, buffer);

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

            flushBuffers();

            return true;
        } else {
            flushBuffers();

            printErrorMessage(response_code);
        }
    }
    return false;
}

bool ClockfyDSImpl::stopTimer(char* stop_time, char* user_id, char* workspace_id) {   
    if (isHttpsClientReadyToUse()) {
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

        httpsClient.print(request);
        int response_code = readResponseHeaders();
        
        flushBuffers();
        
        if (response_code == OK) {
            String response = getResponseBody();
            
            #ifdef DEBUG_API
                Serial.println(F("Success"));
            #endif
            
            return true;
        } else {
            printErrorMessage(response_code);
        }
    }
    return false;
}

#endif