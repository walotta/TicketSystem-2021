<?php
    //echo "1";
    include('function_pool.php');
    $id=$_POST["userId"];
    echo query_privilege($id);
?>