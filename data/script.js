$(document).ready(function(){
    $("#apply").click(function(){
        var valeur = $("#choixTimeZone").val();
        $.post("timeZone",{
            valeurTimeZone: valeur
        });
    });
});

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
