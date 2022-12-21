set(sources ${sources}
    first.cxx
    second.cxx
    third.cxx
    fourth.cxx
    fifth.cxx
    sixth.cxx
    seventh.cxx
    eighth.cxx
    nineth.cxx
    tenth.cxx
    eleventh.cxx
    twelveth.cxx
    thirteenth.cxx
    fourteenth.cxx
)

foreach( taskfile ${sources} )
    # I used a simple string replace, to cut off .cpp.
    string( REPLACE ".cxx" "" taskname ${taskfile} )
    add_executable( ${taskname} ${taskfile} )
    # Make sure YourLib is linked to each app
    #target_link_libraries( ${testname} YourLib )
endforeach( taskfile ${TASK_SOURCE_FILES} )
