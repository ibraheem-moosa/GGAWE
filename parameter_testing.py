def write_param_file(fname, d):
    with open(fname, "w+") as f:
        for k in d:
            f.write(str(k) + " " + str(d[k]) + "\n")

params = dict()
params['routeSetSize'] = 15
params['paSm'] = 0.1
params['prcm'] = 0.0
params['minRouteLength'] = 10
params['maxRouteLength'] = 30
params['minFitnessRatio'] = 0.00001
params['minGen'] = 1
params['maxGen'] = 200
params['stableCount'] = 20
params['run'] = 3
params['maxIter'] = 1
params['beta'] = 0.8
params['e'] = 0.7
params['neighborhoodSize'] = 20
params['elite'] = 2
params['pSwap'] = 1

# to change
# pSm, pDelete, tFit, popSize, elite, pSwap

import subprocess
import time
import sys
default_params = {'pSm' : 0.5, 'paSm' : 0.1, 'tFit' : 80, 'popSize' :10, 'neighborhoodSize' : 20, 'pDelete' : 0.4}
processes = []
output_files = []

def run_progs(var, values):
    params.update(default_params)
    for value in values:
        print('Now running for ' +str(var) + ' ' + str(value))
        if type(var) != type((1,1)):
            params[var] = value
        else:
            for i in range(len(var)):
                params[var[i]] = value[i]
        #print(params['popSize'])
        #x=subprocess.call(['bash','run_heuristics.sh', str(params['popSize'])])
        #print(x)
        #params['elite'] = round(default_params['elite'] * params['popSize'] / default_params['popSize'])
        #params['tFit'] = round(default_params['tFit'] * params['popSize'] / default_params['popSize'])
        param_fname = 'params_' + str(var) + '_' + str(value)
        write_param_file(param_fname, params)
        if(len(sys.argv) > 1):
            out_fname = './outputs/' + sys.argv[1] + '_output_' + str(var) + '_' + str(value)
        else: 
            out_fname = './outputs/output_' + str(var) + '_' + str(value)
        out_file = open(out_fname, 'w+')
        output_files.append(out_file)
        p = subprocess.Popen(['./m1_lbs_heuristics', param_fname], stdout=out_file, stderr=subprocess.DEVNULL)
        processes.append(p)
        if len(processes) == 8:
            prog_teriminated = None
            while prog_teriminated == None:
                for i, p in enumerate(processes):
                    if p.poll() is None:
                        time.sleep(1)
                    else:
                        prog_teriminated = i
                        break
            print(processes[prog_teriminated].args)
            processes.remove(processes[prog_teriminated])

#run_progs('popSize', list(range(15, 65, 5)))
#run_progs('pSm', [0.0, 0.1, 0.2, 0.3, 0.4, 0.5, 0.6, 0.7, 0.8, 0.9, 1.0])
#run_progs('pSm', [0.0, 0.2, 0.4, 0.6, 0.8, 1.0])
#run_progs(('pSm', 'paSm'), [(x, 1.0 - x) for x in [0.0, 0.2, 0.4, 0.6, 0.8, 1.0]])
#run_progs('paSm', [0.0, 0.1, 0.2, 0.3, 0.4])
#run_progs('pDelete', [0.1, 0.3, 0.5, 0.7, 0.9])
run_progs('tFit', list(range(60, 201, 20)))
#run_progs('elite', list(range(1, 20, 2)))
#run_progs('pSwap', [x for x in range(0, 3)])

for p in processes:
    p.wait()
for f in output_files:
    f.close()
