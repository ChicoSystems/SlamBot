/**
 * Author: Isaac Assegai
 * This class is used by sensors to keep a running
 * and moving average of sensor results. This is used
 * to smooth out any bad sensor readings.
 */
#include <Math.h>
 
#ifndef MovingAverage_h
#define MovingAverage_h

template <typename V, int N> class MovingAverage{
public:
   /* Constructor. */
     MovingAverage(V def = 0) : p(0){
        for (int i = 0; i < N; i++){
            samples[i] = def;
            //sum += samples[i];
        }
    }

    /*
     * Adds a new value to the running average and returns the current average.
     */
    V add(V new_sample){
        //sum = sum - samples[p] + new_sample;
        //samples[p++] = new_sample;
        //if (p >= N)
        //    p = 0;
        //V answer = sum / N;
        //Serial.print(" MovingAverage: ");
        
        new_sample = new_sample*(3.14159/180); //translate to radians
        samples[p++] = new_sample; // add to next space
        if (p >= N) p = 0; // circle around to beginning if needed
        V sumOfSines = 0;
        V sumOfCosines = 0;
        for(int i = 0; i < N; i++){ // sum up sins and cosines of angle
          sumOfSines += sin(samples[i]); 
          sumOfCosines += cos(samples[i]);
        }
        
        V answer = atan2(sumOfSines, sumOfCosines);
        answer = answer*(180/3.14159); //translate back to degrees
        for(int i = 0; i < N; i++){
          Serial.print(samples[i]*(180/3.14159));
          Serial.print(" "); 
        }
        if(answer < 0) answer += 360;
        Serial.println(answer);
        return answer;
    }

private:
    V samples[N];
   // V sum;
    int p;
};

#endif
