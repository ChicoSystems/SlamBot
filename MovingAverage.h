/**
 * Author: Isaac Assegai
 * This class is used by sensors to keep a running
 * and moving average of sensor results. This is used
 * to smooth out any bad sensor readings.
 */

template <typename V, int N> class MovingAverage{
public:
   /* Constructor. */
    MovingAverage(V def = 0) : sum(0), p(0){
        for (int i = 0; i < N; i++){
            samples[i] = def;
            sum += samples[i];
        }
    }

    /*
     * Adds a new value to the running average and returns the current average.
     */
    V add(V new_sample)
    {
        sum = sum - samples[p] + new_sample;
        samples[p++] = new_sample;
        if (p >= N)
            p = 0;
        return sum / N;
    }

private:
    V samples[N];
    V sum;
    V p;
};