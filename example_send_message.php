<?php

     // run the native and scripted bubblesort functions
     $start = microtime(true);
     $return_value = posixmq_open("/php_influxdb");

     echo ("return value posixmq_open:" . $return_value . "\n");

     $y = posixmq_send_message("José Funcionou uehueheueh\n");
     $native = microtime(true);

     // show the results
     echo("time_posix_message:   ".($native - $start)." seconds\n");
     echo("return_code: $y\n");
?>
