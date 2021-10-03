class Runnable {
public:
    /**
    * Primary entrypoint for start of execution for the code.
    * @param data input data passed from the runnable interface
    * @param context ExecutionContext struct containing info
    about the execution environment (e.g. PID, TID)
    * @return status code to be passed up to the execution manager
    */
    virtual int run(void *data, ExecutionContext context) = 0;
}