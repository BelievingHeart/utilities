# <INFILE> name of the template sh file
# <OUTFILE> name of the bash executable to be install

function(install_sh)
    set(options "")
    set(oneValueArgs INFILE OUTFILE)
    set(multiValueArgs "")
    cmake_parse_arguments(MY "${options}" "${oneValueArgs}"
                        "${multiValueArgs}" ${ARGN} )

set(IN_FILE ${CMAKE_SOURCE_DIR}/templates/${MY_INFILE})
set(OUT_FILE ${CMAKE_SOURCE_DIR}/bin/${MY_OUTFILE})

configure_file(${IN_FILE} ${OUT_FILE} @ONLY)
execute_process(COMMAND chmod +x ${OUT_FILE})

endfunction()