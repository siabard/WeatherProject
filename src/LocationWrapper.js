'use strict';

import { PermissionsAndroid } from "react-native";
import Geolocation from '@react-native-community/geolocation';

export default class LocationWrapper {
    constructor() {
        this.lat = "";
        this.lon = "";
        this.permission = false;
        this.watchID = 0;
        this.connect();
    }

    set_permission(v) {
        this.permission = v;
    }

    connect() {
        if (this.permission === false) {

            var that = this;
            PermissionsAndroid.request(
                PermissionsAndroid.PERMISSIONS.ACCESS_FINE_LOCATION, {
                'title': 'Location Access Required',
                'message': 'This App needs to Access your location'
            }).then(function (granted) {
                that.set_permission(granted === PermissionsAndroid.RESULTS.GRANTED);
            }).catch(err => alert(err));

        }
    }

    get_coords() {
        // console.log("permission: " + this.permission);
        if (this.permission === true) {
            //alert("callLocation Called");
            Geolocation.getCurrentPosition(
                //Will give you the current location
                (position) => {
                    this.lon = JSON.stringify(position.coords.longitude);

                    //getting the Longitude from the location json
                    this.lat = JSON.stringify(position.coords.latitude);
                    //getting the Latitude from the location json
                },
                (error) => alert(error.message),
                { enableHighAccuracy: false, timeout: 20000, maximumAge: 1000 }
            );
            this.watchID = Geolocation.watchPosition((position) => {
                //Will give you the location on location change
                this.lon = JSON.stringify(position.coords.longitude);
                this.lat = JSON.stringify(position.coords.latitude);
            });
        }
    }

    get_lon() { return (this.lon === "") ? -1.0 : parseFloat(this.lon); }
    get_lat() { return (this.lat === "") ? -1.0 : parseFloat(this.lat); }
}

