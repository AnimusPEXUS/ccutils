
function(ccutils_detect_cpu_os cpu_var os_var)
 set(${cpu_var} "x86_64" )
 set(${os_var} "linux-gnu" )
 return(PROPAGATE ${cpu_var} ${os_var})
endfunction()

