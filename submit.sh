echo "**********************************************************************************************"
echo "THIS IS THE SUBMISSION SCRIPT. PLEASE MAKE SURE YOU SUBMIT THE OUTPUT PA4.tar.gz FILE ON TRITONED"
echo "THIS IS THE SUBMISSION SCRIPT. PLEASE MAKE SURE YOU SUBMIT THE OUTPUT PA4.tar.gz FILE ON TRITONED"
echo "THIS IS THE SUBMISSION SCRIPT. PLEASE MAKE SURE YOU SUBMIT THE OUTPUT PA4.tar.gz FILE ON TRITONED"
echo "THIS IS THE SUBMISSION SCRIPT. PLEASE MAKE SURE YOU SUBMIT THE OUTPUT PA4.tar.gz FILE ON TRITONED"
echo "THIS IS THE SUBMISSION SCRIPT. PLEASE MAKE SURE YOU SUBMIT THE OUTPUT PA4.tar.gz FILE ON TRITONED"
echo "THIS IS THE SUBMISSION SCRIPT. PLEASE MAKE SURE YOU SUBMIT THE OUTPUT PA4.tar.gz FILE ON TRITONED"
echo "THIS IS THE SUBMISSION SCRIPT. PLEASE MAKE SURE YOU SUBMIT THE OUTPUT PA4.tar.gz FILE ON TRITONED"
echo "THIS IS THE SUBMISSION SCRIPT. PLEASE MAKE SURE YOU SUBMIT THE OUTPUT PA4.tar.gz FILE ON TRITONED"
echo "**********************************************************************************************"
echo ""
testsPassed=0
ASNLIB='asnlib'
RED='\033[0;31m'
NC='\033[0m'
submit_pathfinder_and_actor_connections=y
deadline1="Tue Nov 02 23:59:59 PST 2017"
current=$(date)
deadline1_date=$(date +%s -d "$deadline1")
current_date=$(date +%s -d "$current")

if [[ $deadline1_date -ge $current_date ]]; then
    echo -e "${RED}Do you want to submit path finder and actorconnections as well? (y/n)${NC}"
    read submit_pathfinder_and_actor_connections
fi

###################################################################
# function for printing what the common exit values are
# $1 should be the return value
printExitCode ()
{
    # TODO add in cases for common exit values
    if [ -n "$1" ]; then
        echo "Exit code is $1"
    fi
}

runNumNeighborsTest()
{
    graph_file=$1
    nodes_file=$2
    correct_output_file=$3
    student_output_file=$4

    make clean > temp.txt
    make main > temp.txt 2>&1
    if [[ $? != 0 ]] ; then
        echo "main doesn't compile. Exiting"
        head -50 temp.txt
        # make clean > temp.txt
        # rm -f temp.txt
        exit
    fi  
    time timeout $5 ./main $graph_file $nodes_file $student_output_file
    numLinesPair=(`wc -l $correct_output_file`)
    numLinesOut=(`wc -l $student_output_file`)
    if [ "${numLinesPair[0]}" == "${numLinesOut[0]}" ]; then
        diff $student_output_file $correct_output_file > num_neighbors_score.txt 2>&1
        echo "diff $student_output_file $correct_output_file > num_neighbors_score.txt 2>&1"
        RTNVAL=$?
        if [ $RTNVAL == 0 ]; then
            echo "number of neighbors test PASSED"
            #          CURRSCORE=1
            let "testsPassed+=1"
        else
            echo "\"\""
            head -20 num_neighbors_score.txt
            echo "\"\""
            echo -e "${RED}Looks like your output didn't match the reference${NC}"
            echo "Here is the diff exit status in case it's useful"
            printExitCode $RTNVAL
            #          CURRSCORE=0
        fi
    else
        echo -e "${RED}Number of lines in your paths file (${numLinesOut[0]}) does not match the pair file (${numLinesPair[0]})${NC}"
        echo "printing refe file"
        cat $correct_output_file
        echo "printing your file"
        cat $student_output_file
        echo "printing diff"
        diff $correct_output_file $student_output_file > a.txt
        cat a.txt
        rm a.txt
    fi
}
# function for running the test cases
# $1 should be the name of the file to test
# $2 should be u/w/bfs/ufind for unweighted or weighted pathfinder or actorconnections
# $3 should be the timeout for the test
runTest ()
{
    if [ "$2" == "u" ] || [ "$2" == "w" ]; then
        echo ""
        echo "Testing ${1}.tsv on pathfinder with ${2} flag ..."
        echo "time taken:"
        time timeout ${3} ./pathfinder $ASNLIB/${1}.tsv ${2} $ASNLIB/${1}_pair.tsv ${1}_${2}out.tsv > temp.txt 2>&1
        RTNVAL=$?
        printExitCode $RTNVAL
        echo "Verifying your paths ..."
        numLinesPair=(`wc -l $ASNLIB/${1}_pair.tsv`)
        numLinesOut=(`wc -l ${1}_${2}out.tsv`)
        if [ "${numLinesPair[0]}" == "${numLinesOut[0]}" ]; then
            $ASNLIB/modPath.sh ${1}_${2}out.tsv ${1}_${2}out_mod.tsv
            $ASNLIB/pathverifier $ASNLIB/${1}.tsv ${2} $ASNLIB/${1}_pair.tsv ${1}_${2}out_mod.tsv ${1}_${2}score.txt > temp.txt 2>&1
            #cat temp.txt
            CURRSCORE=`tail -1 ${1}_${2}score.txt`
            if [ "$CURRSCORE" == "1" ]; then
                #          echo "${1}.tsv test PASSED (+1)."
                echo "${1}.tsv test PASSED."
                let "testsPassed+=1"
            elif [ "$CURRSCORE" == "0.5" ]; then
                head -50 temp.txt
                echo -e "${RED}Looks like one or more of your paths weren't shortest.${NC}"
                #          echo "You will still get half the credit for this test file (+0.5)."
                echo "You would still get half the credit for this test file."
            elif [ "$CURRSCORE" == "0" ]; then
                head -50 temp.txt
                echo -e "${RED}Looks like one or more of your paths weren't correct.${NC}"
                #          echo "Sorry, but no points for this test file (+0)."
                echo "Sorry, but no points for this test file."
            else
                echo "If you see this message you may not have successfully created an output file."
            fi
        else
            echo -e "${RED}Number of lines in your paths file (${numLinesOut[0]}) does not match the pair file (${numLinesPair[0]})${NC}"
            #        echo "Sorry, but no points for this test file (+0)."
            echo "Sorry, but no points for this test file (+0)."
            #        CURRSCORE=0
        fi
        rm ${1}_${2}out.tsv ${1}_${2}out_mod.tsv ${1}_${2}score.txt >temp.txt 2>&1
        #      totalScore=`echo "$totalScore + $CURRSCORE" | bc`
    elif [ "$2" == "bfs" ] || [ "$2" == "ufind" ]; then
        # TODO actorconnection stuff here
        echo ""
        echo "Testing ${1}.tsv on actorconnections with the ${2} flag ..."
        echo "time taken:"
        time timeout ${3} ./actorconnections $ASNLIB/${1}.tsv $ASNLIB/${1}_pair.tsv ${1}_${2}out.tsv ${2} > temp.txt 2>&1
        RTNVAL=$?
        printExitCode $RTNVAL
        #      echo "Verifying your paths ..."
        numLinesPair=(`wc -l $ASNLIB/${1}_pair.tsv`)
        numLinesOut=(`wc -l ${1}_${2}out.tsv`)
        if [ "${numLinesPair[0]}" == "${numLinesOut[0]}" ]; then
            echo "Diffing reference with your output: 'diff refOutput yourOutput'"
            diff $ASNLIB/${1}_arefout.tsv ${1}_${2}out.tsv > ${1}_${2}score.txt 2>&1
            echo "diff $ASNLIB/${1}_arefout.tsv ${1}_${2}out.tsv > ${1}_${2}score.txt 2>&1"
            RTNVAL=$?
            if [ $RTNVAL == 0 ]; then
                echo "${1}.tsv ${2} test PASSED"
                #          CURRSCORE=1
                let "testsPassed+=1"
            else
                echo "\"\""
                head -20 ${1}_${2}score.txt
                echo "\"\""
                echo "${RED}Looks like your output didn't match the reference${NC}"
                echo "Here is the diff exit status in case it's useful"
                printExitCode $RTNVAL
                #          CURRSCORE=0
            fi
        else
            echo -e "${RED}Number of lines in your paths file (${numLinesOut[0]}) does not match the pair file (${numLinesPair[0]})${NC}"
            echo "printing refe file"
            cat $ASNLIB/${1}_arefout.tsv
            echo "printing your file"
            cat ${1}_${2}out.tsv
            echo "printing diff"
            diff $ASNLIB/${1}_arefout.tsv ${1}_${2}out.tsv > a.txt
            cat a.txt
            rm a.txt
            #        echo "Sorry, but no points for this test file (+0)."
            #        CURRSCORE=0
        fi
        rm ${1}_${2}out.tsv ${1}_${2}out_mod.tsv ${1}_${2}score.txt >temp.txt 2>&1
        #      totalScore=`echo "$totalScore + $CURRSCORE" | bc`
    else
        echo "Wrong argument was given to runTest"
    fi
}
###################################################################
CHECKFILE=main.cpp
# Checking for files in directory #
if [[ -f $CHECKFILE ]] ; then
    echo "$CHECKFILE exists in directory"
else
    echo "MISSING REQUIRED FILE: $CHECKFILE . Exiting"
    exit
fi
if [[ $deadline1_date -lt $current_date ]] || [[ $submit_pathfinder_and_actor_connections != n ]]; then
    CHECKFILE=pathfinder.cpp
    # Checking for files in directory #
    if [[ -f $CHECKFILE ]] ; then
        echo "$CHECKFILE exists in directory"
    else
        echo "MISSING REQUIRED FILE: $CHECKFILE . Exiting"
        exit
    fi
    CHECKFILE=actorconnections.cpp
    # Checking for files in directory #
    if [[ -f $CHECKFILE ]] ; then
        echo "$CHECKFILE exists in directory"
    else
        echo "MISSING REQUIRED FILE: $CHECKFILE . Exiting"
        exit
    fi
    ##################################################################
    CHECKFILE=Report.pdf
    # Checking for files in directory #
    if [[ -f $CHECKFILE ]] ; then
        echo "$CHECKFILE exists in directory"
    else
        echo "MISSING REQUIRED FILE: $CHECKFILE . Be sure to add before your last submission."
        ##################################################################
    fi
fi
##################################################################
CHECKFILE=Makefile
# Checking for files in directory #
if [[ -f $CHECKFILE ]] ; then
    echo "$CHECKFILE exists in directory"
else
    echo "MISSING REQUIRED FILE: $CHECKFILE . Exiting"
    exit
fi
# Check compilation #
echo "Compiling ..."
make clean > temp.txt
make main > temp.txt 2>&1
if [[ $? != 0 ]] ; then
    echo "main doesn't compile. Exiting"
    head -50 temp.txt
    # make clean > temp.txt
    # rm -f temp.txt
    exit
fi
if [[ $deadline1_date -lt $current_date ]] || [[ $submit_pathfinder_and_actor_connections != n ]]; then
    make pathfinder > temp.txt 2>&1
    if [[ $? != 0 ]] ; then
        echo "pathfinder doesn't compile. Exiting"
        head -50 temp.txt
        # make clean > temp.txt
        # rm -f temp.txt
        exit
    fi 
    make actorconnections > temp.txt 2>&1
    if [[ $? != 0 ]] ; then
        echo "actorconnections doesn't compile. Exiting"
        head -50 temp.txt
        # make clean > temp.txt
        # rm -f temp.txt
        exit
    fi 
fi
echo "Code compiles successfully! (warnings may still exist)"
##################################################################
# Create an array of test filenames
declare -a testFiles=("2-node_simple" "3-node_simple" "3-node_3-movie" "3-node_3-movie2" "medium_weighted_graph" "movie_casts_grading")
# Create an array of test modes
declare -a testModes=("u" "w" "bfs" "ufind")
# Create an array of test times
declare -a testTimes=("5" "5" "5" "5" "10" "15"
"5" "5" "5" "5" "10" "15"
"5" "5" "5" "5" "10" "20"
"5" "5" "5" "5" "10" "20")
# Get length of arrays
testFilesLen=${#testFiles[@]}
testModesLen=${#testModes[@]}
totalTests=0
if [[ $deadline1_date -lt $current_date ]] || [[ "$submit_pathfinder_and_actor_connections" != "n" ]]; then
    for (( i=0; i < ${testModesLen}; i++ )); do
        for (( j=0; j < ${testFilesLen}; j++ )); do
            runTest ${testFiles[$j]} ${testModes[$i]} ${testTimes[$totalTests]}
            let "totalTests+=1"
        done
    done
fi

runNumNeighborsTest $ASNLIB/movie_casts.tsv $ASNLIB/test_neighbors.tsv $ASNLIB/test_neighbors_out.tsv test_neighbors_student_out.tsv 5
let "totalTests+=1"

echo ""
echo -e "${RED}Passed $testsPassed tests out of $totalTests total tests${NC}"
##################################################################
rm tmpOut1.txt tmpOut2.txt > /dev/null 2>&1
echo ""

tar_file_gz="PA4.tar.gz"
tar_file="PA4.tar"

input_files="pathfinder.cpp actorconnections.cpp Makefile Report.pdf main.cpp"
if [[ $current_date -le $deadline1_date ]] && [[ "$submit_pathfinder_and_actor_connections" == "n" ]]; then
    input_files="Makefile main.cpp"
fi

tar -zcvf $tar_file_gz $input_files
if [[ $? != 0 ]] ; then
    echo "Unable to create $tar_file_gz! Exit Code: $?"
    exit
fi

echo "Please add any extra file needed for execution:"
echo "\nPlease insert the name of the file you would like to add to the archive -e.g 'addtional_file.hpp' otherwise insert 'q', without quotes and followed by [ENTER]:"
read additional_file

check_if_q_and_compiles() {
    if [[ "$additional_file" = "q" ]]; then
        temp_dir2="temp_dir2/"
        mkdir -p $temp_dir2
        tar -zxvf $tar_file_gz --directory $temp_dir2
        pushd . > temp.txt
        cd $temp_dir2
        make clean > temp.txt
        make main > temp.txt 2>&1
        if [[ $? != 0 ]] ; then
            echo -e "${RED}main doesn't compile. Did you forget to add a file?${NC}"
            head -50 temp.txt
            echo ""
            ls .. --hide $tar_file_gz
            echo ""
            additional_file=""
        else
            if [[ $deadline1_date -lt $current_date ]] || [[ $submit_pathfinder_and_actor_connections != n ]]; then
                make pathfinder > temp.txt 2>&1
                if [[ $? != 0 ]] ; then
                    echo -e "${RED}pathfinder doesn't compile. Did you forget to add a file?${NC}"
                    head -50 temp.txt
                    echo ""
                    ls .. --hide $tar_file_gz
                    echo ""
                    additional_file=""
                else
                    make actorconnections > temp.txt 2>&1
                    if [[ $? != 0 ]] ; then
                        echo -e "${RED}actorconnections doesn't compile. Did you forget to add a file?${NC}"
                        head -50 temp.txt
                        echo ""
                        ls .. --hide $tar_file_gz
                        echo ""
                        additional_file=""
                    fi
                fi
            fi
        fi
        popd > temp.txt
        rm -rf $temp_dir2
    fi
}

check_if_q_and_compiles
while [[ $additional_file != q ]]
do
    gunzip $tar_file_gz
    tar -rf $tar_file $additional_file
    gzip $tar_file
    if [[ $? != 0 ]] ; then
        echo "Unable to add file $additional_file, failed with exitcode: $?"
    else
        echo "$additional_file added to archive."
    fi
    echo "Please insert the name of the file you would like to add to the archive -e.g 'addtional_file.hpp' otherwise insert 'q', without quotes and followed by [ENTER]:"
    read additional_file
    check_if_q_and_compiles       
done
echo "Submission Script Finished"
echo -e "${RED}Please submit the ${tar_file_gz} on tritonEd!${NC}"
