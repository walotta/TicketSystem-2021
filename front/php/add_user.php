<?php
    $userId=$_POST['userId'];
    $userName=$_POST['userName'];
    $mail=$_POST['mail'];
    $psw=$_POST['psw'];
    $pri=$_POST['pri'];

    if($userId==""||$userName==""||$mail==""||$pri==""||$psw=="")
    {
        echo "invaild";
    }else
    {
        $cmd="add_user -c walotta_admin -u $userId -p $psw -n $userName -m $mail -g $pri";
        $socket = socket_create(AF_INET,SOCK_STREAM,SOL_TCP);
        socket_connect($socket,'ticket.walotta.top',9988);
        socket_write($socket,strlen($cmd).$cmd);
        $res = socket_read($socket, 204800);
        socket_close($socket);
        if($res=="-1")echo "failed";
        else echo "success";
    }
?>