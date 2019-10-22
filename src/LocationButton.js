import * as React from "react";
import { Button, PermissionsAndroid } from "react-native";
import Geolocation from '@react-native-community/geolocation';


export default class LocationButton extends React.Component {
    componentDidMount = () => {
        var that = this;
        async function requestLocationPermission() {

            const granted = await PermissionsAndroid.request(
                PermissionsAndroid.PERMISSIONS.ACCESS_FINE_LOCATION, {
                'title': 'Location Access Required',
                'message': 'This App needs to Access your location'
            }
            )
            if (granted === PermissionsAndroid.RESULTS.GRANTED) {
                //To Check, If Permission is granted
                that.callLocation(that);
            } else {
                alert("Permission Denied");
            }

        }
        requestLocationPermission();

    }

    callLocation(that) {
        //alert("callLocation Called");
        Geolocation.getCurrentPosition(
            //Will give you the current location
            (position) => {
                const currentLongitude = JSON.stringify(position.coords.longitude);

                //getting the Longitude from the location json
                const currentLatitude = JSON.stringify(position.coords.latitude);
                //getting the Latitude from the location json
                alert(currentLongitude);
                alert(currentLatitude);
            },
            (error) => alert(error.message),
            { enableHighAccuracy: false, timeout: 20000, maximumAge: 1000 }
        );
        that.watchID = Geolocation.watchPosition((position) => {
            //Will give you the location on location change
            console.log(position);
        });
    }
    componentWillUnmount = () => {
        Geolocation.clearWatch(this.watchID);
    }


    _onPress() {
        this.callLocation(this);
    };

    render() {
        return (
            <Button title="위치" onPress={this._onPress.bind(this)} />
        );
    }
}
