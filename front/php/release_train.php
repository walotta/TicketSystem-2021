<?php
    $trainId=$_POST['trainId'];

    $cmd="release_train -i $trainId";
    $socket = socket_create(AF_INET,SOCK_STREAM,SOL_TCP);
    socket_connect($socket,'ticket.walotta.top',9988);
    socket_write($socket,strlen($cmd).$cmd);
    $res = socket_read($socket, 204800);
    socket_close($socket);
    if($res=="-1")echo "failed";
    else echo "success";
?>