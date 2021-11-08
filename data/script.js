//Blinking speed//
$(document).ready(function(){ // Where page is open, 
    $("#applyBlinking").click(function(){ //waitng user to click BTN 
        var UserDelyLed = $("#choixDelayLed").val(); // GEt the value from choixDelayLed
        $.post("delayLed",{ //creat Post request from page ESP32/delayled
            valDelayLed: UserDelyLed //Send UserDelyLed in argument named valDelayLed
        });
    });
});

//Brightness value//
setInterval(function getData() {
    var xhttp = new XMLHttpRequest();

    xhttp.onreadystatechange = function () {
        if (this.readyState == 4 && this.status == 200) {
            document.getElementById("valeurLuminosite").innerHTML = this.responseText;
        }
    };

    xhttp.open("GET", "lireLuminosite", true);
    xhttp.send();
}, 2000);

//Time Zone//
$(document).ready(function(){
    $("#applyTimeZone").click(function(){ //applyTimeZone ==>Button aplay
        var valeur = $("#userTimeZone").val(); //userTimeZone==> ID List
        $.post("timezone",{ //timezone ==> URL PAGE
            valUserTZ: valeur // valUserTZ to send to ESP32
        });
    });
});
//ON OFF//
function onButton() {
    var xhttp = new XMLHttpRequest();
    xhttp.open("GET", "on", true);
    xhttp.send();
}

function offButton() {
    var xhttp = new XMLHttpRequest();
    xhttp.open("GET", "off", true);
    xhttp.send();
}