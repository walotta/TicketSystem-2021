<?php
    $userId=$_POST['userId'];
    $cmd="logout -u $userId";
    $socket = socket_create(AF_INET,SOCK_STREAM,SOL_TCP);
    socket_connect($socket,'ticket.walotta.top',9988);
    socket_write($socket,strlen($cmd).$cmd);
    $res = socket_read($socket, 204800);
    socket_close($socket);
    if($res=="-1")echo "-1";
    else echo "1";
?>