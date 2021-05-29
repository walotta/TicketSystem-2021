function getCookie(cname)
{
  var name = cname + "=";
  var ca = document.cookie.split(';');
  for(var i=0; i<ca.length; i++) 
  {
    var c = ca[i].trim();
    if (c.indexOf(name)==0) return c.substring(name.length,c.length);
  }
  return "";
}

function check_on_load()
{
    $.ajax({
        type: "POST",
        dataType: "text",
        url: "/php/check_login.php",
        data: {
            key: getCookie("Key")
        },
        success: function(resp)
        {
          if(resp!="1")window.location.replace("/html/index.html");
        },
        error: function()
        {
          window.location.replace("error.walotta.top");
        }
    }
    )
    /*var xmlhttp;
    xmlhttp=new XMLHttpRequest();
    xmlhttp.open("POST","/php/check_login.php",false);
    xmlhttp.setRequestHeader(getCookie("Key"));
    xmlhttp.send();
    var ss;
    ss=xmlhttp.responseText;*/
    //if(ss!="1")
    //window.location.replace("/html/list.html");
    //window.location.replace("error.walotta.top");
};