<?php
    $userId=$_POST['userId'];
    $number=$_POST['number'];

    if($number=="")
    {
        echo "-1";
    }else
    {
        $cmd="refund_ticket -u $userId -n $number";
        $socket = socket_create(AF_INET,SOCK_STREAM,SOL_TCP);
        socket_connect($socket,'ticket.walotta.top',9988);
        socket_write($socket,strlen($cmd).$cmd);
        $res = socket_read($socket, 204800);
        socket_close($socket);
        echo $res;
    }
?>