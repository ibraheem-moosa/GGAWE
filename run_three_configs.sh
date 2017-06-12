python3 edit_a_line_in_file.py parameter_testing.py 30 "default_params = {'pSm' : 0.0, 'paSm' : 0.1, 'tFit' : 80, 'popSize' :10, 'neighborhoodSize' : 20, 'pDelete' : 0.4}"
python3 parameter_testing.py lbs_pasm_0.1_pbm_0.9_param_tuning
python3 edit_a_line_in_file.py parameter_testing.py 30 "default_params = {'pSm' : 0.5, 'paSm' : 0.0, 'tFit' : 80, 'popSize' :10, 'neighborhoodSize' : 20, 'pDelete' : 0.4}"
python3 edit_a_line_in_file.py parameter_testing.py 72 "run_progs('pDelete', [0.0, 0.1, 0.2, 0.3, 0.4, 0.5, 0.6, 0.7, 0.8, 0.9, 1.0])"
python3 parameter_testing.py lbs_psm_0.5_pbm_0.5_param_tuning
python3 edit_a_line_in_file.py parameter_testing.py 30 "default_params = {'pSm' : 0.4, 'paSm' : 0.0, 'tFit' : 80, 'popSize' :10, 'neighborhoodSize' : 20, 'pDelete' : 0.4}"
python3 parameter_testing.py lbs_psm_0.4_pbm_0.6_param_tuning
