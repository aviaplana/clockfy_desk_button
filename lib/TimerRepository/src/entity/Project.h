#ifndef PROJECT_H
#define PROJECT_H

/*
{
    "id": "5e4e42156fe9056cfb00252",
    "name": "Project",
    "hourlyRate": null,
    "clientId": "",
    "workspaceId": "5e4e38caq7668c16662b303a",
    "billable": true,
    "memberships": [
        {
            "userId": "5a0ab5acb2d987125438b60f",
            "hourlyRate": null,
            "targetId": "5e4e38f156fe9056cfb00252",
            "membershipType": "PROJECT",
            "membershipStatus": "ACTIVE"
        }
    ]
    "color": "#4CAF50",
    "estimate": {
        "estimate":  "PT0S",
        "type":  "AUTO"
    },
    "archived": false,
    "duration": "PT0S",
    "clientName": "",
    "note": "",
    "template": false,
    "public": false
}   
*/

struct Project {
    char id[30];
    char name[50];
    char color[10];
};

#endif