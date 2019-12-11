#include <algorithm>
#include <fcntl.h>
#include <map>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <string>
#include <sys/stat.h>
#include <vector>

#ifdef _WIN32
#include <io.h>
#else
#include <unistd.h>
#define O_BINARY 0
#endif

static const double testdata[] = { // "7"
	0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000,
	0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000,
	0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000,
	0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000,
	0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000,
	0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000,
	0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000,
	0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.329412, 0.725490, 0.623529, 0.592157, 0.235294, 0.141176, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000,
	0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.870588, 0.996078, 0.996078, 0.996078, 0.996078, 0.945098, 0.776471, 0.776471, 0.776471, 0.776471, 0.776471, 0.776471, 0.776471, 0.776471, 0.666667, 0.203922, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000,
	0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.262745, 0.447059, 0.282353, 0.447059, 0.639216, 0.890196, 0.996078, 0.882353, 0.996078, 0.996078, 0.996078, 0.980392, 0.898039, 0.996078, 0.996078, 0.549020, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000,
	0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.066667, 0.258824, 0.054902, 0.262745, 0.262745, 0.262745, 0.231373, 0.082353, 0.925490, 0.996078, 0.415686, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000,
	0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.325490, 0.992157, 0.819608, 0.070588, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000,
	0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.086275, 0.913725, 1.000000, 0.325490, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000,
	0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.505882, 0.996078, 0.933333, 0.172549, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000,
	0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.231373, 0.976471, 0.996078, 0.243137, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000,
	0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.521569, 0.996078, 0.733333, 0.019608, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000,
	0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.035294, 0.803922, 0.972549, 0.227451, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000,
	0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.494118, 0.996078, 0.713726, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000,
	0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.294118, 0.984314, 0.941176, 0.223529, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000,
	0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.074510, 0.866667, 0.996078, 0.650980, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000,
	0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.011765, 0.796078, 0.996078, 0.858824, 0.137255, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000,
	0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.149020, 0.996078, 0.996078, 0.301961, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000,
	0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.121569, 0.878431, 0.996078, 0.450980, 0.003922, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000,
	0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.521569, 0.996078, 0.996078, 0.203922, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000,
	0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.239216, 0.949020, 0.996078, 0.996078, 0.203922, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000,
	0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.474510, 0.996078, 0.996078, 0.858824, 0.156863, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000,
	0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.474510, 0.996078, 0.811765, 0.070588, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000,
	0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000,
};

struct Matrix {
	int row = 0;
	int col = 0;
	std::vector<double> values;

	void make(int r, int c)
	{
		row = r;
		col = c;
		values.clear();
		values.resize(row * col);
	}

	void make(int r, int c, std::initializer_list<double> const &list)
	{
		row = r;
		col = c;
		values.clear();
		values = list;
	}

	void make(int r, int c, double const *data)
	{
		row = r;
		col = c;
		values.clear();
		values.reserve(row * col);
		for (int i = 0; i < row; i++) {
			for (int j = 0; j < col; j++) {
				values.push_back(*data);
				data++;
			}
		}
	}

	double &at(int r, int c)
	{
		return values[col * r + c];
	}

	double at(int r, int c) const
	{
		return values[col * r + c];
	}

	void dot(Matrix const &other, Matrix *out)
	{
		*out = {};
		Matrix const &a = *this;
		Matrix const &b = other;
		int n = a.col;
		if (n == b.row) {
			int nrow = a.row;
			int ncol = b.col;
			out->make(nrow, ncol);
			for (int col = 0; col < ncol; col++) {
				for (int row = 0; row < nrow; row++) {
					for (int i = 0; i < n; i++) {
						out->at(row, col) += a.at(row, i) * b.at(i , col);
					}
				}
			}
		}
	}

	void add(Matrix const &other)
	{
		size_t n = std::min(values.size(), other.values.size());
		for (size_t i = 0; i < n; i++) {
			values[i] += other.values[i];
		}
	}

	void sigmoid()
	{
		size_t n = values.size();
		for (size_t i = 0; i < n; i++) {
			values[i] = 1 / (1 + exp(-values[i]));
		}
	}

	void softmax(Matrix *out)
	{
		*out = {};
		double c = 0;
		size_t n = values.size();
		for (size_t i = 0; i < n; i++) {
			c = std::max(c, values[i]);
		}
		std::vector<double> exp_a(n);
		double sum_exp_a = 0;
		for (size_t i = 0; i < n; i++) {
			double v = exp(values[i] - c);
			exp_a[i] = v;
			sum_exp_a += v;
		}
		out->make(1, n);
		for (size_t i = 0; i < n; i++) {
			out->at(0, i) = exp_a[i] / sum_exp_a;
		}
	}
};

struct Network {
	std::map<std::string, Matrix> map;

	void parse(char const *begin, char const *end)
	{
		map.clear();
		char const *ptr = begin;
		char const *head = begin;
		std::string name;
		Matrix matrix;
		while (1) {
			int c = 0;
			if (ptr < end) {
				c = (unsigned char)*ptr;
			}
			if (c == ',' || c == '\r' || c == '\n' || c == '[' || c == 0) {
				if (head < ptr) {
					std::string s(head, ptr);
					matrix.values.push_back(strtod(s.c_str(), nullptr));
				}
				ptr++;
				head = ptr;
				if (c == '[' || c == 0) {
					if (!name.empty() && !matrix.values.empty()) {
						auto it = map.insert(map.end(), std::pair<std::string, Matrix>(name, {}));
						std::swap(it->second, matrix);
					}
					if (c == 0) break;
				}
				if (c == '[') {
					int n;
					for (n = 0; ptr + n < end; n++) {
						if (ptr[n] == ']') {
							std::string s(ptr, n);
							std::vector<char> tmp(n);
							n++;
							int row = 0;
							int col = 0;
							int i = sscanf(s.c_str(), "%s %u %u", tmp.data(), &row, &col);
							if (i == 3) {
								matrix = {};
								matrix.row = row;
								matrix.col = col;
								name.assign(tmp.data());
							}
							break;
						}
					}
					ptr += n;
					head = ptr;
				}
			} else {
				ptr++;
			}
		}
	}
};

struct Layer {
	Matrix weight;
	Matrix bias;

	bool load(Network const &network, std::string const &w_name, std::string const &b_name)
	{
		auto i = network.map.find(w_name);
		if (i != network.map.end()) {
			weight = i->second;
		}

		auto j = network.map.find(b_name);
		if (j != network.map.end()) {
			bias = j->second;
		}

		if (weight.row > 0 && weight.col > 0 && weight.row * weight.col == weight.values.size()) {
			// ok
		} else {
			fprintf(stderr, "invalid network weight data: %s\n", w_name.c_str());
			return false;
		}

		if (bias.row > 0 && bias.col > 0 && bias.row * bias.col == bias.values.size()) {
			// ok
		} else {
			fprintf(stderr, "invalid network bias data: %s\n", b_name.c_str());
			return false;
		}

		return true;
	}
};

bool readfile(char const *path, std::vector<char> *out)
{
	bool ok = false;
	out->clear();
	int fd = open(path, O_RDONLY | O_BINARY);
	if (fd != -1) {
		struct stat st;
		if (fstat(fd, &st) == 0 && st.st_size > 0) {
			out->resize(st.st_size);
			if (read(fd, out->data(), out->size()) == out->size()) {
				ok = true;
			}
		}
		close(fd);
	}
	return ok;
}

int main()
{
	std::vector<char> vec;
	if (!readfile("network.txt", &vec) || vec.empty()) {
		fprintf(stderr, "failed to load the network file\n");
	}

	Network network;
	{
		char const *begin = vec.data();
		char const *end = begin + vec.size();
		network.parse(begin, end);
	}

	struct Model {
		Layer layer1;
		Layer layer2;
		Layer layer3;
	};

	Model model;

	auto LoadLayer = [&](std::string const &w_name, std::string const &b_name, Layer *layer){
		if (!layer->load(network, w_name, b_name)) {
			exit(1);
		}
	};
	LoadLayer("W1", "B1", &model.layer1);
	LoadLayer("W2", "B2", &model.layer2);
	LoadLayer("W3", "B3", &model.layer3);

	Matrix in;
	in.make(1, 28 * 28, testdata);

	Matrix a1;
	Matrix a2;
	Matrix a3;
	Matrix score;

	in.dot(model.layer1.weight, &a1);
	a1.add(model.layer1.bias);
	a1.sigmoid();

	a1.dot(model.layer2.weight, &a2);
	a2.add(model.layer2.bias);
	a2.sigmoid();

	a2.dot(model.layer3.weight, &a3);
	a3.add(model.layer3.bias);

	a3.softmax(&score);

	for (size_t i = 0; i < score.values.size(); i++) {
		printf("[%2d] %3.6f\n", (int)i, score.values[i]);
	}

	return 0;
}