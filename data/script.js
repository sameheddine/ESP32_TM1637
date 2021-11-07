//Blinking speed//
$(document).ready(function(){
    $("#appliquer").click(function(){
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
    $("#apply").click(function(){
        var valeur = $("#userTimeZone").val();
        $.post("timeZone",{
            valeurTimeZone: valUserTimeZone
        });
    });
});
//ON OFF//
function onButton() {
    var xhttp = new XMLHttpRequest();
    xhttp.open("GET", "on", true);//Methode GET, "on" on ouvre la page /on de l'esp 32, true c'est pour l'asynchrone. 
    xhttp.send();
}


function offButton() {
    var xhttp = new XMLHttpRequest();
    xhttp.open("GET", "off", true);//Methode GET, "off" on ouvre la page /on de l'esp 32, true c'est pour l'asynchrone. 
    xhttp.send();
}