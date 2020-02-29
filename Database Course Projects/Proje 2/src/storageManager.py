import os
import sys

RECORD_SIZE = 20 # fields
SYSTEM_PAGE_SIZE = 5 # records
DATA_PAGE_SIZE = 16 # records
SYSTEM_FILE_SIZE = 100 # pages
DATA_FILE_SIZE = 100 # pages

empty_syscat_record = "type_name|true|0" + ("|@"*RECORD_SIZE)
empty_data_record = "true" + ("|@"*RECORD_SIZE)
empty_syscat_page = ((empty_syscat_record+'$')*SYSTEM_PAGE_SIZE)[:-1]
empty_data_page = ((empty_data_record+'$')*DATA_PAGE_SIZE)[:-1]

relative_path = "2018400279/src/"


def strip(str):
	return str.rstrip().lstrip()


# syscat_record = "type_name | is_empty | num_fields | field1 | field2 ..."
# data_record = "is_empty | field1 | field2 ..."

class DataBase:

	def __init__(self, syscat_name):
		self.syscat_name = syscat_name

	
	def reset(self, database_name, owner_name):
		with open(relative_path+self.syscat_name, 'w+') as file:
			file.write(database_name + " " + owner_name + '\n')
			for i in range(SYSTEM_FILE_SIZE):
				file.write(empty_syscat_page)
				file.write('\n')

	
	def create_type(self, type_name, num_fields, fields):

		with open(relative_path+self.syscat_name, 'r') as file:

			found = False
			pages = file.readlines()

			for page_index in range(len(pages)):
				
				if page_index == 0:
					continue

				page_str = pages[page_index]
				page = page_str.split('$')

				for record_index in range(len(page)):
					record_str = page[record_index]
					record = record_str.split('|')
					if record[1] == "true":
						self.create_data_file(type_name)
						found = True
						record[0] = type_name
						record[1] = "false"
						record[2] = str(num_fields)
						for i in range(num_fields):
							record[i+3] = fields[i]
						break

				if found:
					page[record_index] = "|".join(record)
					pages[page_index] = "$".join(page)
					break

			if not found:
				print("NOT ENOUGH SPACE!!!")

			pages = list(map(strip, pages))
			new_file = '\n'.join(pages)

		with open(relative_path+self.syscat_name, 'w') as file:
			file.write(new_file)


	def delete_type(self, type_name):

		with open(relative_path+self.syscat_name, 'r') as file:

			found = False
			pages = file.readlines()

			for page_index in range(len(pages)):
				
				if page_index == 0:
					continue

				page_str = pages[page_index]
				page = page_str.split('$')

				for record_index in range(len(page)):
					record_str = page[record_index]
					record = record_str.split('|')
					if record[0] == type_name:
						os.remove(relative_path+type_name+".db")
						found = True
						record[0] = "type_name"
						record[1] = "true"
						record[2] = '0'
						for i in range(RECORD_SIZE):
							record[i+3] = '@'
						break

				if found:
					page[record_index] = "|".join(record)
					pages[page_index] = "$".join(page)
					break

			pages = list(map(strip, pages))
			new_file = '\n'.join(pages)

		with open(relative_path+self.syscat_name, 'w') as file:
			file.write(new_file)


	def list_type(self):

		result = []
		with open(relative_path+self.syscat_name, 'r') as file:

			pages = file.readlines()

			for page_index in range(len(pages)):
				
				if page_index == 0:
					continue

				page_str = pages[page_index]
				page = page_str.split('$')

				for record_index in range(len(page)):

					record_str = page[record_index]
					record = record_str.split('|')
					if record[1] == "false":
						result.append(record[0])
		return sorted(result)


	def create_data_file(self, type_name):
		with open(relative_path+type_name+".db", 'w+') as file:
			for i in range(DATA_FILE_SIZE):
				file.write(empty_data_page)
				file.write('\n')


	def create_record(self, type_name, fields):
		try:
			with open(relative_path+type_name+".db", 'r') as file:
				found = False
				pages = file.readlines()

				for page_index in range(len(pages)):

					page_str = pages[page_index]
					page = page_str.split('$')

					for record_index in range(len(page)):
						record_str = page[record_index]
						record = record_str.split('|')
						if record[0] == "true":
							found = True
							record[0] = "false"
							for i in range(len(fields)):
								record[i+1] = fields[i]
							break

					if found:
						page[record_index] = "|".join(record)
						pages[page_index] = "$".join(page)
						break

				if not found:
					print("NOT ENOUGH SPACE!!!")

				pages = list(map(strip, pages))
				new_file = '\n'.join(pages)

		except FileNotFoundError:
			return

		with open(relative_path+type_name+".db", 'w') as file:
			file.write(new_file)


	def delete_record(self, type_name, pk):
		try:
			with open(relative_path+type_name+".db", 'r') as file:
				found = False
				pages = file.readlines()

				for page_index in range(len(pages)):

					page_str = pages[page_index]
					page = page_str.split('$')

					for record_index in range(len(page)):
						record_str = page[record_index]
						record = record_str.split('|')
						if record[0] == "false" and record[1] == pk:
							found = True
							record[0] = "true"
							for i in range(RECORD_SIZE):
								record[i+1] = '@'
							break

					if found:
						page[record_index] = "|".join(record)
						pages[page_index] = "$".join(page)
						break

				pages = list(map(strip, pages))
				new_file = '\n'.join(pages)

		except FileNotFoundError:
			return

		with open(relative_path+type_name+".db", 'w') as file:
			file.write(new_file)		


	def update_record(self, type_name, pk, fields):
		try:
			with open(relative_path+type_name+".db", 'r') as file:
				found = False
				pages = file.readlines()

				for page_index in range(len(pages)):

					page_str = pages[page_index]
					page = page_str.split('$')

					for record_index in range(len(page)):
						record_str = page[record_index]
						record = record_str.split('|')
						if record[0] == "false" and record[1] == pk:
							found = True
							for i in range(len(fields)):
								record[i+2] = fields[i]
							break

					if found:
						page[record_index] = "|".join(record)
						pages[page_index] = "$".join(page)
						break

				pages = list(map(strip, pages))
				new_file = '\n'.join(pages)

		except FileNotFoundError:
			return

		with open(relative_path+type_name+".db", 'w') as file:
			file.write(new_file)


	def search_record(self, type_name, pk):
		result = []
		found = False
		try:
			with open(relative_path+type_name+".db", 'r') as file:
				pages = file.readlines()

				for page_index in range(len(pages)):

					page_str = pages[page_index]
					page = page_str.split('$')

					for record_index in range(len(page)):
						record_str = page[record_index]
						record = record_str.split('|')
						if record[0] == "false" and record[1] == pk:
							found = True
							for i in range(RECORD_SIZE):
								if record[i+1] == '@':
									break
								result.append(record[i+1])
							break
					
					if found:
						break

		except FileNotFoundError:
			return []

		return result


	def list_records(self, type_name):
		result = []
		try:
			with open(relative_path+type_name+".db", 'r') as file:
				pages = file.readlines()

				for page_index in range(len(pages)):

					page_str = pages[page_index]
					page = page_str.split('$')

					for record_index in range(len(page)):
						record_str = page[record_index]
						record = record_str.split('|')
						if record[0] == "false":
							result.append([])
							for i in range(RECORD_SIZE):
								if record[i+1] == '@':
									break
								result[-1].append(record[i+1])
		except FileNotFoundError:
			return []

		result.sort(key=lambda x: x[0])
		return result


if __name__ == '__main__':

	my_database = DataBase("SystemCatalog.db")

	if sys.argv[1] == "reset":
		my_database.reset(sys.argv[2], sys.argv[3])

	else:
		input_file = sys.argv[1]
		output_file = sys.argv[2]

		with open(input_file, 'r') as file_input:
			with open(output_file, 'w+') as file_output:
				for line in file_input:
					if len(line) <= 1:
						continue
					line = line.strip().split(' ')
					query = (line[0] + line[1]).lower()

					if query == "createtype":
						type_name = line[2]
						num_fields = line[3]
						fields = line[4:]
						my_database.create_type(type_name, int(num_fields), fields)

					elif query == "deletetype":
						type_name = line[2]
						my_database.delete_type(type_name)

					elif query == "listtype":
						result = my_database.list_type()
						if len(result) != 0:
							for e in result:
								file_output.write(e)
								file_output.write('\n')

					elif query == "createrecord":
						type_name = line[2]
						fields = line[3:]
						my_database.create_record(type_name, fields)

					elif query == "deleterecord":
						type_name = line[2]
						pk = line[3]
						my_database.delete_record(type_name, pk)

					elif query == "updaterecord":
						type_name = line[2]
						pk = line[3]
						fields = line[4:]
						my_database.update_record(type_name, pk, fields)

					elif query == "searchrecord":
						type_name = line[2]
						pk = line[3]
						result = my_database.search_record(type_name, pk)
						if len(result) > 0:
							file_output.write(" ".join(result))
							file_output.write('\n')


					elif query == "listrecord":
						type_name = line[2]
						result = my_database.list_records(type_name)
						if len(result) > 0:
							for e in result:
								file_output.write(" ".join(e))
								file_output.write('\n')

