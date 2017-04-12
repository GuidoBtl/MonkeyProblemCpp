/*******************************************************************************
Name:        monkey.cpp

Purpose:     Program simulating monkeys crossing a canyon.

Summary:     Different monkey processes synchronize through semaphores to cross
             the canyon without collisions nor deadlocks.

Author:      Guido Bertolotti

Revision:    1.1

Date:        2017/04/12

Description: Monkey Problem

             We want to know if African monkeys can be taught about deadlocks.
             She locates a deep canyon and fastens a rope across it, so the
             monkeys can cross hand-over-hand.

             Passage along the rope follows these rules:

             Several monkeys can cross at the same time, provided that they are
             all going in the same direction.

             If eastward moving and westward moving monkeys ever get onto the
             rope at the same time, a deadlock will result (the monkeys will get
             stuck in the middle) because it is impossible for one monkey to
             climb over another one while suspended over the canyon.

             If a monkey wants to cross the canyon, he must check to see that no
             other monkey is currently crossing in the opposite direction.

             Your solution should avoid starvation. When a monkey that wants to
             cross to the east arrives at the rope and finds monkeys crossing to
             the west, the monkey waits until the rope in empty, but no more
             westward moving monkeys are allowed to start until at least one
             monkey has crossed the other way.

             For this exercise, you are to write a program to simulate activity
             for this canyon crossing problem:

             Simulate each monkey as a separate process.
             Altogether, a lot of monkeys, whatever you want, will cross the
             canyon, with a random number generator specifying whether they are
             eastward moving or westward moving.
             Use a random number generator, so the time between monkey arrivals
             is between 1 and 8 seconds.

             Each monkey takes 1 second to get on the rope. (That is, the
             minimum inter-monkey spacing is 1 second.)
             All monkeys travel at the same speed. Each traversal takes exactly
             4 seconds, after the monkey is on the rope.

             Enjoy it!

Run example: $ gcc monkey.cpp -o monkey -lstdc++

             $ ./monkey 10

             MONKEY PROBLEM

             Symbols description:

             TIM     = TIMe in seconds from the beginning
             Q       = west and east Queue semaphores
             D       = west and east Direction semaphores
             ROPE    = the ROPE, of course
             MESSAGE = monkey state MESSAGE
             >       = eastward going monkey
             <       = westward going monkey

             TIM:   Q D  ROPE  D Q   MESSAGE

             000:   | |  ----  | |<  Monkey  1 arrived to the queue.
             000:   | |  ----  |<|   Monkey  1 is ready to cross.
             000:   | |  ---- <| |   Monkey  1 is going to the rope.
             001:  >| |  ----  | |   Monkey  2 arrived to the queue.
             001:   |>|  ----  | |   Monkey  2 is ready to cross.
             001:   | |  ---<  | |   Monkey  1 is on the rope crossing the canyon.
             001:  >| |  ----  | |   Monkey  3 arrived to the queue.
             001:  >| |  ----  | |   Monkey  4 arrived to the queue.
             001:  >| |  ----  | |   Monkey  5 arrived to the queue.
             001:  >| |  ----  | |   Monkey  6 arrived to the queue.
             001:  >| |  ----  | |   Monkey  7 arrived to the queue.
             005:   | | <----  | |   Monkey  1 finished crossing the canyon.
             005:   | |> ----  | |   Monkey  2 is going to the rope.
             006:   | |  >---  | |   Monkey  2 is on the rope crossing the canyon.
             006:   |>|  ----  | |   Monkey  3 is ready to cross.
             006:   | |> ----  | |   Monkey  3 is going to the rope.
             007:   | |  >---  | |   Monkey  3 is on the rope crossing the canyon.
             007:   |>|  ----  | |   Monkey  4 is ready to cross.
             007:   | |> ----  | |   Monkey  4 is going to the rope.
             008:   | |  >---  | |   Monkey  4 is on the rope crossing the canyon.
             008:   |>|  ----  | |   Monkey  5 is ready to cross.
             008:   | |> ----  | |   Monkey  5 is going to the rope.
             009:   | |  >---  | |   Monkey  5 is on the rope crossing the canyon.
             009:   |>|  ----  | |   Monkey  6 is ready to cross.
             009:   | |> ----  | |   Monkey  6 is going to the rope.
             009:   | |  ----  | |<  Monkey  8 arrived to the queue.
             009:   | |  ----  |<|   Monkey  8 is ready to cross.
             009:   | |  ----  | |<  Monkey  9 arrived to the queue.
             009:   | |  ----  | |<  Monkey 10 arrived to the queue.
             010:   | |  >---  | |   Monkey  6 is on the rope crossing the canyon.
             010:   | |  ----> | |   Monkey  2 finished crossing the canyon.
             010:   |>|  ----  | |   Monkey  7 is ready to cross.
             011:   | |  ----> | |   Monkey  3 finished crossing the canyon.
             012:   | |  ----> | |   Monkey  4 finished crossing the canyon.
             013:   | |  ----> | |   Monkey  5 finished crossing the canyon.
             014:   | |  ----> | |   Monkey  6 finished crossing the canyon.
             014:   | |  ---- <| |   Monkey  8 is going to the rope.
             015:   | |  ---<  | |   Monkey  8 is on the rope crossing the canyon.
             015:   | |  ----  |<|   Monkey  9 is ready to cross.
             019:   | | <----  | |   Monkey  8 finished crossing the canyon.
             019:   | |> ----  | |   Monkey  7 is going to the rope.
             020:   | |  >---  | |   Monkey  7 is on the rope crossing the canyon.
             024:   | |  ----> | |   Monkey  7 finished crossing the canyon.
             024:   | |  ---- <| |   Monkey  9 is going to the rope.
             025:   | |  ---<  | |   Monkey  9 is on the rope crossing the canyon.
             025:   | |  ----  |<|   Monkey 10 is ready to cross.
             025:   | |  ---- <| |   Monkey 10 is going to the rope.
             026:   | |  ---<  | |   Monkey 10 is on the rope crossing the canyon.
             029:   | | <----  | |   Monkey  9 finished crossing the canyon.
             030:   | | <----  | |   Monkey 10 finished crossing the canyon.
             030:Parent process: last child terminated.
             Parent process: goodbye.

*******************************************************************************/

/*******************************************************************************
 Includes:
*******************************************************************************/
#include <iostream>                     // cout, cerr
#include <iomanip>                      // setw(), setfill()
#include <cstdio>                       // sprintf()
#include <cstdlib>                      // srand(), rand() ...
#include <unistd.h>                     // fork()
#include <sys/wait.h>                   // wait()
#include <sys/types.h>                  // sem_t, pid_t
#include <semaphore.h>                  // sem_open(), sem_wait() ...
#include <fcntl.h>                      // O_CREAT, O_EXCL
#include <ctime>                        // time()
#include <csignal>                      // signal()
#include <cerrno>                       // errno
#include <stdexcept>                    // throw

/*******************************************************************************
 Defines:
*******************************************************************************/
// main:
#define MIN_MONKEYS_NUMBER          1   // Minimum number of monkeys to be generated in a run, must be greater than 0
#define MAX_MONKEYS_NUMBER          999 // Maximum number of monkeys to be generated in a run

// function monkeyCrossing: canyon semaphores definitions:
#define SEM_NAME_QUEUE_EAST_WEST    "/Q_EW"
#define SEM_NAME_QUEUE_WEST_EAST    "/Q_WE"
#define SEM_NAME_DIR_EAST_WEST      "/DIR_EW"
#define SEM_NAME_DIR_WEST_EAST      "/DIR_WE"
#define SEM_NAME_DIR_EXCL           "/DIR_EXCL"
#define SEM_NAME_FREE_PLACES        "/FREE_PLACES"
#define SEM_NAME_FREE_PLACES_EXCL   "/FREE_PLACES_EXCL"
#define MAX_FREE_PLACES             5   // Maximum number of places for monkeys crossing at the same time

// function monkeyCrossing: monkeys definitions:
#define MIN_MONKEYS_NUMBER_IN_GROUP 1   // Minimum number of monkeys to be generated per monkeys group
#define MAX_MONKEYS_NUMBER_IN_GROUP 6   // Maximum number of monkeys to be generated per monkeys group
#define MIN_SECONDS_BETWEEN_GROUPS  1   // Minimum number of seconds between monkeys groups
#define MAX_SECONDS_BETWEEN_GROUPS  8   // Maximum number of seconds between monkeys groups

// class Monkey:
#define SECONDS_TO_GO_TO_ROPE       1   // Seconds to go to the rope
#define SECONDS_TO_CROSS_CANYON     4   // Seconds to cross the canyon

// class Logger: semaphores definitions:
#define SEM_NAME_LOGGER_DEFAULT     "/LOGGER_DEFAULT"

// class Sem: semaphores definitions:
#define SEM_PERMISSIONS             0644
#define SEM_FLAGS_CREATION          (O_CREAT | O_EXCL)
#define SEM_FLAGS_USE               0


/*******************************************************************************
Class:       Time

Description: Time functions wrappers.

             time_t Time::seconds()
             returns the number of seconds since the Epoch
             (1970-01-01 00:00:00 +0000 UTC).
             In case of error an exception is thrown instead of returning
             an error value.
*******************************************************************************/
class Time
{
    public:
        static time_t seconds();
};

time_t Time::seconds()
{
    time_t currentTime;

    if ((currentTime = time(NULL)) == ((time_t)-1))
    {
        throw std::runtime_error("time(NULL) failed");
    }

    return currentTime;
}


/*******************************************************************************
Class:       Sem

Description: Semaphore wrapper class.
             Semaphores are objects of this class.
             They throw exceptions instead of returning error values, and
             are easier to use than the original library functions.

             Sem(const char* semName, bool useExisting = true, int semValue = 1)
             is the semaphore constructor.
             semName = semaphore name (always starting with "/", e.g. "/MY_SEM";
             useExisting = false to create a new semaphore, true (default) to use existing one;
             semValue = new semaphore initial value, defaulting at 1.

             The methods names are self-explaining (see semaphore.h man pages).
*******************************************************************************/
class Sem
{
    public:
        Sem(const char* semName, bool useExisting = true, int semValue = 1);

        void   wait    ();
        void   tryWait ();
        void   post    ();
        int    getValue();

        ~Sem();

    protected:
        const char* semName;
        sem_t*      sem;
        bool        usingExisting;

        // Id of the process which created the semaphore, the only one which can destroy it:
        pid_t  creatorPid;
};

Sem::Sem(const char* semName, bool useExisting, int semValue)
{
    if (useExisting)
    {
        if ((sem = sem_open(semName, SEM_FLAGS_USE)) < 0)
        {
            throw std::runtime_error("sem_open(semName, SEM_FLAGS_USE) failed");
        }
    }
    else // !useExisting
    {
        if (semValue <= 0)
        {
            throw std::invalid_argument("semValue <= 0");
        }

        // Clean up semaphore in case it hang after a previous process break/abort:
        if (sem_unlink(semName) == 0)
        {
            std::cerr << "Warning (previous process aborted?): semaphore " << semName << " was hanging." << std::endl;
        }

        if ((sem = sem_open(semName, SEM_FLAGS_CREATION, SEM_PERMISSIONS, semValue)) == SEM_FAILED)
        {
            throw std::runtime_error("sem_open(semName, SEM_FLAGS_CREATION, SEM_PERMISSIONS, semValue) failed");
        }

        // Store the creating process id:
        creatorPid = getpid();
    }

    usingExisting = useExisting;
    this->semName = semName;
}

Sem::~Sem()
{
    if (usingExisting)
    {
        if (sem_close(sem) < 0)
        {
            throw std::runtime_error("(sem_close(sem) failed");
        }
    }
    else // !usingExisting
    {
        // semaphore can only be destroyed by the process which created it:
        if (getpid() == creatorPid)
        {
            if (sem_unlink(semName) < 0)
            {
                throw std::runtime_error("sem_unlink(semName) failed");
            }
        }
    }
}

void Sem::wait()
{
    if (sem_wait(sem) < 0)
    {
        throw std::runtime_error("sem_wait failed");
    }
}

void Sem::tryWait()
{
    if (sem_trywait(sem) < 0 && errno != EAGAIN)
    {
        throw std::runtime_error("sem_trywait(sem) failed");
    }
}

void Sem::post()
{
    if (sem_post(sem) < 0)
    {
        throw std::runtime_error("sem_post(sem) failed");
    }
}

int Sem::getValue()
{
    int value;

    if (sem_getvalue(sem, &value) < 0)
    {
        throw std::runtime_error("sem_getvalue(sem, &value) failed");
    }

    return value;
}


/*******************************************************************************
Class:       Logger

Description: This class prints log messages, with or without timestamps.
             It takes care of concurrency issues and avoids mixing output
             from different processes.

Methods:     static Logger& getInstance(const char* semNameLogger)
             is the instance getter for singleton static methods.
             It is not necessary to explicitly call this method.
             But this class uses a semaphore whose name is defined in
             #define SEM_NAME_LOGGER_DEFAULT.
             It is possible to use a different name by calling
             getInstance("/DESIRED_SEMAPHORE_NAME")
             before any other method of this class is firstly used.

             static void Logger::resetZeroTime()
             explicitly defines current time as time 0 for next timestamps.
             Can be called multiple times. If not called, time 0 is set as
             the time in which a method of this class is firstly used.

             static void Logger::print(const char* message, bool includeTimestamp)
             writes a message to the log output, with or without timestamp.
*******************************************************************************/
class Logger
{
    public:
        // static instance getter for singleton static methods:
        static Logger& getInstance(const char* semNameLogger = SEM_NAME_LOGGER_DEFAULT);

        // static method, wrapper for resetZeroTimeD():
        static void resetZeroTime();

        // static method, wrapper for printD():
        static void print(const char* message, bool includeTimestamp = false);

    protected:
        // A semaphore is used to avoid mixing messages on log output:
        Sem semLogger;

        // Zero time for timestamps:
        time_t zeroTime;

        // Stores current time as zero time to calculate relative time for timestamps:
        void resetZeroTimeD();

        // Writes a message to the log output, with or without timestamp:
        void printD(const char* message, bool includeTimestamp);

    private:
        // Constructor is private for singleton:
        Logger(const char* semNameLogger);
};

Logger& Logger::getInstance(const char* semNameLogger)
{
    // Singleton is instantiated on first use:
    static Logger logger(semNameLogger);

    return logger;
}

Logger::Logger(const char* semNameLogger):
        // Construct semaphore:
        semLogger(semNameLogger, false)
{
    resetZeroTimeD();
}

void Logger::resetZeroTimeD()
{
    zeroTime = Time::seconds();
}

void Logger::resetZeroTime()
{
    Logger::getInstance().resetZeroTimeD();
}

void Logger::printD(const char* message, bool includeTimestamp)
{
    // CRITICAL SECTION: BEGIN (guarantees exclusive access for std::clog to avoid mixing messages):
    semLogger.wait();

    if (includeTimestamp)
    {
        // Print timestamp:
        std::clog << std::right << std::setfill('0') << std::setw(3) << (int)((Time::seconds() - zeroTime) % 1000) << ":";
    }

    // Print message:
    std::clog << message << std::endl;

    // CRITICAL SECTION: END:
    semLogger.post();
}

void Logger::print(const char* message, bool includeTimestamp)
{
    Logger::getInstance().printD(message, includeTimestamp);
}


/*******************************************************************************
Class:       Canyon

Description: canyon equipped with rope and semaphores.
             Semaphores allow processes to cross the canyon without deadlocks.

Methods:     The names of inline getter methods have clear meanings.
*******************************************************************************/
class Canyon
{
    public:
        Canyon(const char* semNameQueueAB, const char* semNameQueueBA,    const char* semNameDirAB, const char* semNameDirBA,
               const char* semNameDirExcl, const char* semNameFreePlaces, const char* semNameDirOtherExcl, const int freePlaces);

        const char* getSemNameQueueAB(void)
        {
            return semNameQueueAB;
        };

        const char* getSemNameQueueBA(void)
        {
            return semNameQueueBA;
        };

        const char* getSemNameDirAB(void)
        {
            return semNameDirAB;
        };

        const char* getSemNameDirBA(void)
        {
            return semNameDirBA;
        };

        const char* getSemNameDirExcl(void)
        {
            return semNameDirExcl;
        };

        const char* getSemNameFreePlaces(void)
        {
            return semNameFreePlaces;
        };

        const char* getSemNameDirOtherExcl(void)
        {
            return semNameDirOtherExcl;
        };

        const int  getMaxFreePlaces(void)
        {
            return freePlaces;
        };

    protected:
        // Semaphores names:
        const char* semNameQueueAB;         // Semaphore for the queue of processes wanting to cross from A to B
        const char* semNameQueueBA;         // Semaphore for the queue of processes wanting to cross from B to A
        const char* semNameDirAB;           // Semaphore for the process ready to cross from A to B
        const char* semNameDirBA;           // Semaphore for the process ready to cross from B to A
        const char* semNameDirExcl;         // Semaphore to allow exclusive (atomic) access for operations on semDirAB and semDirBA semaphores
        const char* semNameFreePlaces;      // Semaphore to count remaining free places for processes crossing at the same time
        const char* semNameDirOtherExcl;    // Semaphore to allow exclusive (atomic) access for operations which lock/unlock the semaphore
                                            // of the process's opposite travelling direction

        // Semaphores:
        Sem semQueueAB;
        Sem semQueueBA;
        Sem semDirAB;
        Sem semDirBA;
        Sem semDirExcl;
        Sem semFreePlaces;
        Sem semDirOtherExcl;

        // Free places to cross the canyon at the same time:
        int freePlaces;
};

Canyon::Canyon(const char*     semNameQueueAB,
               const char*     semNameQueueBA,
               const char*     semNameDirAB,
               const char*     semNameDirBA,
               const char*     semNameDirExcl,
               const char*     semNameFreePlaces,
               const char*     semNameDirOtherExcl,
               const int       freePlaces):
               // Construct named semaphores:
               semQueueAB     (semNameQueueAB,      false),
               semQueueBA     (semNameQueueBA,      false),
               semDirAB       (semNameDirAB,        false),
               semDirBA       (semNameDirBA,        false),
               semDirExcl     (semNameDirExcl,      false),
               semFreePlaces  (semNameFreePlaces,   false, freePlaces),
               semDirOtherExcl(semNameDirOtherExcl, false)
{
    // Store semaphores names:
    this->semNameQueueAB      = semNameQueueAB;
    this->semNameQueueBA      = semNameQueueBA;
    this->semNameDirAB        = semNameDirAB;
    this->semNameDirBA        = semNameDirBA;
    this->semNameDirExcl      = semNameDirExcl;
    this->semNameFreePlaces   = semNameFreePlaces;
    this->semNameDirOtherExcl = semNameDirOtherExcl;

    // Store free places:
    this->freePlaces = freePlaces;
}


/*******************************************************************************
Class:       Monkey

Description: monkey is able to cross canyons concurrently with other monkeys
             and avoids deadlocks by using canyon's semaphores.

Methods:     void cross(Canyon canyon, Direction direction)
             is used to order the monkey to cross a canyon in the
             desired direction.
*******************************************************************************/
class Monkey
{
    public:
        // Directions in which the monkey can cross a canyon:
        enum Direction
        {
            DIR_A_TO_B,
            DIR_B_TO_A,
            DIR_NUMBER      // Total number of directions
        };

        Monkey(const int MonkeyId);

        void cross(Canyon canyon, Direction direction);

    protected:
        // Monkeys states during canyon crossing:
        enum State
        {
            STATE_IN_QUEUE,
            STATE_READY_TO_CROSS,
            STATE_GOING_TO_ROPE,
            STATE_CROSSING_ON_ROPE,
            STATE_FINISHED_CROSSING,
            STATE_NUMBER    // Total number of states
        };
        int  monkeyId;

        // Logs a direction-and-state depending message on the logging output:
        void logMessage(Direction direction, State state);
};

Monkey::Monkey(const int monkeyId)
{
    this->monkeyId = monkeyId;
}

void Monkey::cross(Canyon canyon, Direction direction)
{
    // Open canyon semaphores, taking into account crossing direction:
    // semQueueOwn     : Semaphore for the queue of monkeys wanting to cross in own direction
    // semQueueOther   : Semaphore for the queue of monkeys wanting to cross in the other direction
    // semDirOwn       : Semaphore for the monkey ready to cross in own direction
    // semDirOther     : Semaphore for the monkey ready to cross in the other direction
    // semDirExcl      : Semaphore to allow exclusive (atomic) access for operations on semDirOwn and semDirOther semaphores
    // semFreePlaces   : Semaphore to count remaining free places for monkeys crossing at the same time
    // semDirOtherExcl : Semaphore to allow exclusive (atomic) access for operations involving semDirOther semaphore

    Sem semQueueOwn    (direction == DIR_A_TO_B ? canyon.getSemNameQueueAB() : canyon.getSemNameQueueBA());
    Sem semQueueOther  (direction == DIR_A_TO_B ? canyon.getSemNameQueueBA() : canyon.getSemNameQueueAB());
    Sem semDirOwn      (direction == DIR_A_TO_B ? canyon.getSemNameDirAB()   : canyon.getSemNameDirBA());
    Sem semDirOther    (direction == DIR_A_TO_B ? canyon.getSemNameDirBA()   : canyon.getSemNameDirAB());
    Sem semDirExcl     (canyon.getSemNameDirExcl());
    Sem semFreePlaces  (canyon.getSemNameFreePlaces());
    Sem semDirOtherExcl(canyon.getSemNameDirOtherExcl());

    int maxFreePlaces = canyon.getMaxFreePlaces();

    // Now the monkey crosses the canyon, waiting on the semaphores and passing through various states:

    logMessage(direction, STATE_IN_QUEUE);

    semQueueOwn.wait();          // Wait on the queue in own direction

    logMessage(direction, STATE_READY_TO_CROSS);

    semDirExcl.wait();           // CRITICAL SECTION 1 (semDirExcl): BEGIN (can be executed by one process at a time)
    semDirOwn.wait();            //     Wait for allowed crossing in own direction
    semFreePlaces.wait();        //     One more monkey crossing: subtract one to the available places
    semDirOtherExcl.wait();      //     CRITICAL SECTION 2 (semDirOtherExcl): BEGIN (can be executed by one process at a time
                                 //                                           and is mutually exclusive with critical section 3)
    semDirOther.tryWait();       //         If not already forbidden, forbid crossing in the other direction
    semDirOtherExcl.post();      //     CRITICAL SECTION 2 (semDirOtherExcl): END
    semDirExcl.post();           // CRITICAL SECTION 1 (semDirExcl): END

    logMessage(direction, STATE_GOING_TO_ROPE);

    sleep(SECONDS_TO_GO_TO_ROPE);

    logMessage(direction, STATE_CROSSING_ON_ROPE);

    semDirOwn.post();            // Unlock crossing in own direction
    semQueueOwn.post();          // Unlock the queue in own direction

    sleep(SECONDS_TO_CROSS_CANYON);

    logMessage(direction, STATE_FINISHED_CROSSING);

    semFreePlaces.post();        // One less monkey crossing: add one to the available places

    // If tryWait(semDirOther) is executed by somebody else just in between the following getValue(semFreePlaces)
    // and post(semDirOther), semDirOther will result incorrectly unlocked.
    // So both sections 2 and 3 must be protected by a mutually exclusive access, implemented by semDirOtherExcl:

    semDirOtherExcl.wait();      // CRITICAL SECTION 3 (semDirOtherExcl): BEGIN (can be executed by one process at a time
                                 //                                       and is mutually exclusive with critical section 2)
    if (semFreePlaces.getValue() == maxFreePlaces)
                                 //     If crossing cleared
    {
        semDirOther.post();      //         Unlock crossing in the other direction
    }
    semDirOtherExcl.post();      // CRITICAL SECTION 3 (semDirOtherExcl): END
}

void Monkey::logMessage(Direction direction, State state)
{
    static const char *drawing[DIR_NUMBER][STATE_NUMBER] =
    {
        {
        //            1         2         3
        //   123456789012345678901234567890 <- drawing string length ruler
            " | |  ----  | |<",                     // DIR_A_TO_B, STATE_IN_QUEUE
            " | |  ----  |<| ",                     // DIR_A_TO_B, STATE_READY_TO_CROSS
            " | |  ---- <| | ",                     // DIR_A_TO_B, STATE_GOING_TO_ROPE
            " | |  ---<  | | ",                     // DIR_A_TO_B, STATE_CROSSING_ON_ROPE
            " | | <----  | | "                      // DIR_A_TO_B, STATE_FINISHED_CROSSING
        },
        {
            ">| |  ----  | | ",                     // DIR_B_TO_A, STATE_IN_QUEUE
            " |>|  ----  | | ",                     // DIR_B_TO_A, STATE_READY_TO_CROSS
            " | |> ----  | | ",                     // DIR_B_TO_A, STATE_GOING_TO_ROPE
            " | |  >---  | | ",                     // DIR_B_TO_A, STATE_CROSSING_ON_ROPE
            " | |  ----> | | "                      // DIR_B_TO_A, STATE_FINISHED_CROSSING
        }
    };

    static const size_t MAX_DRAWING_LEN = 16;       // See drawing string length ruler above

    static const char *description[STATE_NUMBER] =
    {
    //            1         2         3         4
    //   1234567890123456789012345678901234567890 <- description string length ruler
        "arrived to the queue",                     // STATE_IN_QUEUE
        "is ready to cross",                        // STATE_READY_TO_CROSS
        "is going to the rope",                     // STATE_GOING_TO_ROPE
        "is on the rope crossing the canyon",       // STATE_CROSSING_ON_ROPE
        "finished crossing the canyon"              // STATE_FINISHED_CROSSING
    };

    static const size_t MAX_DESCRIPTION_LEN = 34;   // See description string length ruler above

    static const size_t MAX_EXTRA_CHARS_LEN = 16;   // Added spaces, periods etc. in message (from snprintf format string below)

    static const size_t MAX_SIZE = MAX_DRAWING_LEN + MAX_DESCRIPTION_LEN + MAX_EXTRA_CHARS_LEN + 1;

    // Use local scope (stack allocated), limited size message buffer instead of heap allocated string (improves MISRA compliance):
    char message[MAX_SIZE];

    // Print message depending on direction and state:
    if (snprintf(message, MAX_SIZE, "  %s  Monkey %2d %s.", drawing[direction][state], monkeyId, description[state]) < 0)
    {
        throw std::runtime_error("snprintf(message, MAX_SIZE, ...) failed");
    }

    Logger::print(message, true);
}


/*******************************************************************************
 Functions prototypes:
*******************************************************************************/
int  monkeysCrossing(long monkeysToBeGenerated);
int  main(int argc, char **argv);

/*******************************************************************************
 Functions implementation:
*******************************************************************************/

/*******************************************************************************
Procedure:   int  monkeysCrossing()

Parameters:  long monkeysToBeGenerated: number of monkeys to be generated.

Description: this procedure:
             - prepares the environment for the monkeys,
             - generates monkeys processes and orders them to cross the canyon.
*******************************************************************************/
int monkeysCrossing(long monkeysToBeGenerated)
{
    int               monkeyId;
    int               monkeysInGroup;
    int               lastMonkeyIdInGroup;
    int               secondsBeforeNextGroup;
    Monkey::Direction direction;
    pid_t             pid;

    try
    {
        // Instantiate canyon object:
        Canyon        canyon(SEM_NAME_QUEUE_EAST_WEST,  SEM_NAME_QUEUE_WEST_EAST,
                             SEM_NAME_DIR_EAST_WEST,    SEM_NAME_DIR_WEST_EAST,
                             SEM_NAME_DIR_EXCL,         SEM_NAME_FREE_PLACES,
                             SEM_NAME_FREE_PLACES_EXCL, MAX_FREE_PLACES);

        // Initialize random number generator at a different seed each time the program is run:
        srand(Time::seconds());

        // Don't wait for children termination (only last child termination will be waited for):
        if (signal(SIGCHLD, SIG_IGN) == SIG_ERR)
        {
            throw std::runtime_error("signal(SIGCHLD, SIG_IGN) failed");
        }

        // Print header:
        Logger::print("\n MONKEY PROBLEM\n");
        Logger::print(" Symbols description:\n");
        Logger::print(" TIM     = TIMe in seconds from the beginning");
        Logger::print(" Q       = west and east Queue semaphores");
        Logger::print(" D       = west and east Direction semaphores");
        Logger::print(" ROPE    = the ROPE, of course");
        Logger::print(" MESSAGE = monkey state MESSAGE");
        Logger::print(" >       = eastward going monkey");
        Logger::print(" <       = westward going monkey");
        Logger::print("\nTIM:   Q D  ROPE  D Q   MESSAGE\n");

        Logger::resetZeroTime();

        // Monkey generation loop:
        monkeyId = 0;
        while (monkeyId < monkeysToBeGenerated)
        {
            direction = (Monkey::Direction)(rand() % Monkey::DIR_NUMBER);  // Direction to cross for the monkeys of this group
            monkeysInGroup = (rand() % (MAX_MONKEYS_NUMBER_IN_GROUP - MIN_MONKEYS_NUMBER_IN_GROUP + 1)) + MIN_MONKEYS_NUMBER_IN_GROUP;

            if (monkeyId + monkeysInGroup > monkeysToBeGenerated)
            {
                // monkeyId would overflow monkeysToBeGenerated: reduce monkeysInGroup to the maximum allowed:
                monkeysInGroup = monkeysToBeGenerated - monkeyId;
            }

            lastMonkeyIdInGroup = monkeyId + monkeysInGroup;

            // Monkey group generation loop:
            while (monkeyId < lastMonkeyIdInGroup)
            {
                monkeyId++;

                if (monkeyId == monkeysToBeGenerated)
                {
                    // This is the last child: parent process will wait for its termination, so SIGCHLD must not be ignored any more:
                    if (signal(SIGCHLD, SIG_DFL) == SIG_ERR)
                    {
                        throw std::runtime_error("signal(SIGCHLD, SIG_DFL) failed");
                    }
                }

                // Monkey process generation:
                pid = fork();

                if (pid == 0)
                {
                    // Child process

                    // Instantiate the monkey object:
                    Monkey monkey(monkeyId);

                    // Order the monkey to cross the canyon in the desired direction:
                    monkey.cross(canyon, direction);

                    // child exits:
                    exit(EXIT_SUCCESS);
                }
                else if (pid < 0)
                {
                    throw std::runtime_error("fork() failed");
                }
            }

            if (monkeyId < monkeysToBeGenerated)
            {
                secondsBeforeNextGroup = rand() % (MAX_SECONDS_BETWEEN_GROUPS - MIN_SECONDS_BETWEEN_GROUPS + 1) + MIN_SECONDS_BETWEEN_GROUPS;
                sleep((unsigned int)secondsBeforeNextGroup);
            }
        }

        // Wait termination of last child process:
        while (wait((int*)NULL) > 0);

        Logger::print("Parent process: last child terminated.", true);
        Logger::print("Parent process: goodbye.");

        return EXIT_SUCCESS;
    }
    catch (const std::exception& e)
    {
        std::cerr << "Exception: " << e.what() << std::endl;
        exit(EXIT_FAILURE);
    }
}


/*******************************************************************************
Procedure:   int main()

Parameters:  int argc:    number of arguments.
             char **argv: string arguments values.
                          argv[1] contains the number of monkeys to be generated.

Description: receives the number of monkeys to be generated and calls
             the monkeyCrossing function.
*******************************************************************************/
int main (int argc, char **argv)
{
    long monkeysToBeGenerated;

    // Pointer to the char after the digits:
    char* pEnd;

    // argv[1] contains the number of monkeys to be generated in this run. Read it:
    if ((argc != 2)
        ||
        ((monkeysToBeGenerated = strtol(argv[1], &pEnd, 10)) < MIN_MONKEYS_NUMBER)
        ||
        (monkeysToBeGenerated > MAX_MONKEYS_NUMBER)
        ||
        (*pEnd != '\0'))    // There are extra characters after the digits
    {
        std::cerr << "Usage: " << argv[0] << " NUMBER_OF_MONKEYS" << std::endl;
        std::cerr << "NUMBER_OF_MONKEYS must be in range [" << MIN_MONKEYS_NUMBER << ".." << MAX_MONKEYS_NUMBER << "]" << std::endl;
        return(EXIT_FAILURE);
    }

    return monkeysCrossing(monkeysToBeGenerated);
}
