<?php
    $jsonString=$_POST['send_msg'];
    //$jsonString=str_replace("'",  '"',$jsonString);
    $dateList=json_decode($jsonString,true);
    $cmd="add_train -i ".$dateList['trainId']." -n ".$dateList['stationNum']." -m ".$dateList['seatNum']." -x ".$dateList['startTime']." -y ".$dateList['type']." -d ".$dateList['saleBegin']."|".$dateList['saleEnd'];
    $stationList=$dateList['stationList'];
    $priceList=$dateList['priceList'];
    $travelTimeList=$dateList['travelTimeList'];
    $stopoverList=$dateList['stopoverList'];

    $cmd=$cmd." -s ";
    for($i=0;$i<count($stationList);$i++)
    {
        if($i!=count($stationList)-1)$cmd=$cmd.$stationList[$i].'|';
        else $cmd=$cmd.$stationList[$i];
    }

    $cmd=$cmd." -p ";
    for($i=0;$i<count($priceList);$i++)
    {
        if($i!=count($priceList)-1)$cmd=$cmd.$priceList[$i].'|';
        else $cmd=$cmd.$priceList[$i];
    }

    $cmd=$cmd." -t ";
    echo $travelTimeList;
    for($i=0;$i<count($travelTimeList);$i++)
    {
        if($i!=count($travelTimeList)-1)$cmd=$cmd.$travelTimeList[$i].'|';
        else $cmd=$cmd.$travelTimeList[$i];
    }

    $cmd=$cmd." -o ";
    for($i=0;$i<count($stopoverList);$i++)
    {
        if($i!=count($stopoverList)-1)$cmd=$cmd.$stopoverList[$i].'|';
        else $cmd=$cmd.$stopoverList[$i];
    }
    
    $socket = socket_create(AF_INET,SOCK_STREAM,SOL_TCP);
    socket_connect($socket,'ticket.walotta.top',9988);
    socket_write($socket,strlen($cmd).$cmd);
    $res = socket_read($socket, 204800);
    socket_close($socket);
    if($res=="-1"||$res=="parse error!")echo "failed";
    else echo "success";
?>