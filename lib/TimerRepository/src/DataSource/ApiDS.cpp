#include "ApiDS.h"

ApiDS::ApiDS() {

}

Project** ApiDS::getProjects() {
    WiFiClientSecure client = getConnectedClient();
    String endpoint = "/workspaces/{workspaceId}/projects";


    String request = "GET" + endpoint + " HTTP/1.1\r\n" +
            "X-Api-Key: " + API_KEY + "\r\n" +
            "content-type: application/json\r\n" +
            "Host: " + host + "\r\n" +
            "Connection: close\r\n\r\n";

    client.print(request);

    while (client.connected()) {
        String line = client.readStringUntil('\n');

        if (line == "\r") {
            break;
        }
    }

    String response;

    while(client.available()){        
        response += client.readStringUntil('\n'); 
    }       

    Serial.println(response); 

    return 0;
}

BearSSL::WiFiClientSecure ApiDS::getConnectedClient() {
    WiFiClientSecure httpsClient;
    httpsClient.setFingerprint(fingerprint);
    httpsClient.setTimeout(timeout); // 15 Seconds
    delay(1000);

    unsigned int retry_counter = 0; //retry counter

    while (!httpsClient.connect(host, https_port) &&
            (retry_counter < max_retry)) {
        retry_counter++;
    }
    

    if(retry_counter == max_retry) {
        // Couldn't connect!
        //return -1;
    }

    return httpsClient;
}
