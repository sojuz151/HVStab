#ifndef POLYFIT_H_
#define POLYFIT_H_

#ifdef __cplusplus
extern "C" {
#endif

int polyfit(const float* const dependentValues,
            const float* const independentValues,
            unsigned int        countOfElements,
            unsigned int        order,
            float*             coefficients);


#ifdef __cplusplus
}
#endif

#endif
