//Blinking speed//
$(document).ready(function(){
    $("#applyBlinking").click(function(){
        var valeur = $("#choixDelayLed").val();
        $.post("delayLed",{
            valeurDelayLed: valeur
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