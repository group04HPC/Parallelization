import os
import numpy as np
import scipy as sp
import matplotlib.pyplot as plt
import pandas as pd
from scipy import stats
import seaborn as sns
from prettytable import PrettyTable
from prettytable import MARKDOWN
from prettytable import MSWORD_FRIENDLY
import re

# processes, read, SCC, write, elapsed
config = {
			'seqKey': "NP-00",
			'filenameRegex': "SIZE-[0-9]+-NP-[0-9]{2}",
			'folderRegex':"SIZE-[0-9]+",
			"cols":{
				'read':{

					'jpg':False,
					'computeSpeedup':False,

				},
				'SCC':{

					'jpg':False,
					'computeSpeedup':False,

				},
				'write':{

					'jpg':False,
					'computeSpeedup':False,

				},
				'elapsed':{

					'jpg':False,
					'computeSpeedup':True,
					
				}
			},

			"table":{
				"header": ['Version','Processes','ReadFromFile','SCC','WriteToFile','Elapsed','Speedup','Efficiency'],
			},
			"plot":{
				"x_from_table":"Processes",
				"y_from_table":"Speedup",
			},
			"calcComplExpr":""
		}

def _extract(path_to_folder,plot_columns):
	prev = os.getcwd()
	os.chdir(path_to_folder)

	#List diresctory
	filenames =  [f for f in os.listdir('.') if os.path.isfile(f)]
	if not os.path.exists("jpg"):
		os.mkdir("jpg")

	#Remove not csv files
	#"SIZE-[0-9]+-NTH-[0-9]{2}-O[0-9]-?[0-9]*"
	filenames = [f for f in os.listdir('.') if f.endswith(".csv") and re.match(config["filenameRegex"],f) ]

	filenames = sorted(filenames)
	means = {}
	
	for filename in filenames:
		file_mean = {}
		print('Processing : ' + filename)
		ds = pd.read_csv(filename)
		for col in plot_columns.keys():
			print('Processing : ' + filename + ", Col : " + col)
			if 'skipForFile' in plot_columns[col] and re.match(plot_columns[col]['skipForFile'], filename):
				print('SKIPPED : ' + filename + ", Col : " + col)
				file_mean[col] = 0
				continue
			#extract the selected column
			x_data = ds[col]
			#compute gaussian mean
			mean,std=stats.norm.fit(x_data)
			#compute mean as usual, to use when only few measures are taken
			np_mean = np.mean(x_data)

			#68,3% = P{ μ − 1,00 σ < X < μ + 1,00 σ }
			x_data = ds[(ds[col] < (mean + std)) & (ds[col] > (mean - std))][col]
			mean,std=stats.norm.fit(x_data)
			file_mean[col] = mean if np_mean == mean else np_mean
			
			if plot_columns[col]['jpg']:
				sns.histplot(x_data, kde=True)
				plt.savefig("jpg/" + str(col)+ "_" + filename.split('.')[0] + ".jpg")
				plt.close()
			
		means[filename] = file_mean
	os.chdir(prev)
	return means

def _compute_speedup(t,tp,nt,psize):
    speedup = t/tp
    efficiency = t/(tp*nt)
    return speedup,efficiency

def _make_table(header,rows,print_table=False,save=True,name=""):
	if save and not name:
		raise Exception("No filename to save file")
	x = PrettyTable()
	x.field_names = header
	x.add_rows(rows)
	if save:
		_save_table(x,name)
	if print_table:
		print(x)
	return x

def _save_table(table,filename):
	with open(filename,"w") as table_file:
		#table.set_style(MARKDOWN)
		table.set_style(MSWORD_FRIENDLY)
		data = table.get_string()
		table_file.write(data)

def _plot_from_table(header,rows,save=True,name="",show_plot=False):
	if save and not name:
		raise Exception("No filename to save file")

	x = [0]
	y = [0]
	try:
		x_from_table = config["plot"]["x_from_table"]
		y_from_table = config["plot"]["y_from_table"]
		speedup_pos = config["table"]["header"].index(y_from_table) #header.index("Speedup")
		thread_pos = config["table"]["header"].index(x_from_table) #header.index("Threads")
	except Exception as e:
		print("config table or plot error")

	for row in rows[1:]:
		x.append(row[thread_pos])
		y.append(row[speedup_pos])

	x_th = np.array(x)
	fig, ax = plt.subplots(figsize=(12, 8))
	tx = np.arange(2, 8, 0.05)
	ty = tx/np.log2(tx)
	ax.plot(tx, ty, color='green', label='Theoretical')
	ax.plot(x_th, y, 'ro-', label='Experimental')
	ax.plot(x_th, x_th, color='blue', label='Ideal')
	#same as y_th, bisection
	plt.style.use('seaborn-whitegrid')

	plt.autoscale(enable=True, axis='x', tight=True)
	plt.autoscale(enable=True, axis='y', tight=True)	

	plt.legend()
	plt.xlabel(x_from_table)
	plt.ylabel(y_from_table)
	if show_plot:
		plt.show()
	if save:
		plt.savefig(name)
	plt.close()

def extraction(root=os.path.join(os.path.dirname(os.path.realpath(__file__)),"measure/"), cols=config["cols"], threads=[0,1,2,4,8]):
	print("Listing folder for problem size")
	folders =  [f for f in os.listdir(root) if (os.path.isdir(os.path.join(root,f)) and re.match(config['folderRegex'],f))]
	print(f"Found folders : {folders}")

	for folder in folders:
		print(f"Folder : {folder}")
		joined_path = os.path.join(root,folder)
		means = _extract(joined_path,cols)
		header = {'values':config["table"]["header"]}
		cells = {'values':[]}
		nt = -1
		for filename_key in means:
			cell = []
			splitted_filename = filename_key.split("-")
			if config["seqKey"] in filename_key:
				seq = means[filename_key]['elapsed']
				nt = 1
				cell.append('Serial')
				cell.append(nt)
			else:
				nt = int(splitted_filename[3].split('.')[0])
				cell.append('Parallel')
				cell.append(nt)

			for col in cols:
				cell.append(means[filename_key][col])
				if cols[col]['computeSpeedup']:
					psize = splitted_filename[1]
					speedup,efficiency = _compute_speedup(seq,means[filename_key][col],nt,psize)
					cell.append(speedup)
					cell.append(efficiency)
			cells['values'].append(cell)
		
		splitted_folder = folder.split("-")
		size = splitted_folder[1]
		table_filename = joined_path + "/psize-" + size + "-table.csv"
		plot_filename = joined_path + "/jpg/speedup-" + str(size) + ".jpg"

		table = _make_table(header['values'],cells['values'],name=table_filename)
		_plot_from_table(header["values"],cells["values"],name=plot_filename)

if __name__ == "__main__":
	extraction(os.path.join(os.path.dirname(os.path.realpath(__file__)),"Tarjan/"))
	extraction(os.path.join(os.path.dirname(os.path.realpath(__file__)),"Kosaraju/"))