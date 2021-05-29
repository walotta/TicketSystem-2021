<?php
    function query_privilege($userId)
    {
        $cmd="query_privilege -u $userId";
        $socket = socket_create(AF_INET,SOCK_STREAM,SOL_TCP);
        socket_connect($socket,'ticket.walotta.top',9988);
        socket_write($socket,strlen($cmd).$cmd);
        $pri = socket_read($socket, 204800);
        socket_close($socket);
        return $pri;
    }
?>