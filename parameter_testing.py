def write_param_file(fname, d):
    with open(fname, "w+") as f:
        for k in d:
            f.write(str(k) + " " + str(d[k]) + "\n")

params = dict()
params['routeSetSize'] = 15
params['prcm'] = 0.0
params['minRouteLength'] = 10
params['maxRouteLength'] = 30
params['minFitnessRatio'] = 0.00001
params['minGen'] = 1
params['maxGen'] = 100
params['stableCount'] = 20
params['run'] = 5
params['maxIter'] = 1
params['beta'] = 0.8
params['e'] = 0.7

# to change
# pSm, pDelete, tFit, popSize, elite, pSwap

import subprocess
import time
import sys
default_params = {'pSm' : 0.3, 'pDelete' : 0.4, 'tFit' : 10, 'popSize' : 30, 'elite' : 2, 'pSwap' : 1}
processes = []
output_files = []

def run_progs(var, values):
    params.update(default_params)
    for value in values:
        print('Now running for ' + var + ' ' + str(value))
        params[var] = value
        #print(params['popSize'])
        #x=subprocess.call(['bash','run_heuristics.sh', str(params['popSize'])])
        #print(x)
        #params['elite'] = round(default_params['elite'] * params['popSize'] / default_params['popSize'])
        #params['tFit'] = round(default_params['tFit'] * params['popSize'] / default_params['popSize'])
        param_fname = 'params_' + var + '_' + str(value)
        write_param_file(param_fname, params)
        if(len(sys.argv) > 1):
            out_fname = './outputs/' + sys.argv[1] + '_output_' + var + '_' + str(value)
        else: 
            out_fname = './outputs/output_' + var + '_' + str(value)
        out_file = open(out_fname, 'w+')
        output_files.append(out_file)
        p = subprocess.Popen(['./test', param_fname], stdout=out_file, stderr=subprocess.DEVNULL)
        processes.append(p)
        if len(processes) == 2:
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
#run_progs('pDelete', [0.0, 0.1, 0.2, 0.3, 0.4, 0.5, 0.6, 0.7, 0.8, 0.9, 1.0])
#run_progs('tFit', list(range(2, 20)))
#run_progs('elite', list(range(1, 20, 2)))
run_progs('pSwap', [x for x in range(0, 3)])

for p in processes:
    p.wait()
for f in output_files:
    f.close()
