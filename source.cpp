#include <iostream>
#include <cmath>
using namespace std;

class Polynomial {
protected:
	class Term {
	public:
		int exponent;
		int coefficient;
		Term* next;
		Term(int exp, int coeff, Term* n) {
			exponent = exp;
			coefficient = coeff;
			next = n;
		}
		friend class Polynomial;
	};

public:
	Polynomial() {
		term = new Term(0, 0, nullptr);
	}

	Polynomial(const Polynomial& p) {
		Term* q;
		Term* terms = nullptr;
		bool flag = true;
		for (q = p.term; q != nullptr; q = q->next) {
			if (flag) {
				flag = false;
				terms = new Term(q->exponent, q->coefficient, nullptr);
				term = terms;
			}
			else {
				terms->exponent = q->exponent;
				terms->coefficient = q->coefficient;
			}
			if (q->next != nullptr) {
				terms->next = new Term(0, 0, nullptr);
				terms = terms->next;
			}
		}
	}

	~Polynomial() {
		while (true) {
			if (term->next == nullptr)
				break;
			Term* p = term;
			term = term->next;
			delete p;
		}
		delete term;
	}

	Polynomial& operator=(const Polynomial& p) {
		while (true) {
			if (term->next == nullptr)
				break;
			Term* pterm = term;
			term = term->next;
			delete pterm;
		}
		delete[] term;
		Term* q;
		Term* terms;
		bool flag = true;
		for (q = p.term; q != nullptr; q = q->next) {
			if (flag) {
				flag = false;
				terms = new Term(q->exponent, q->coefficient, nullptr);
				term = terms;
			}
			else {
				terms->exponent = q->exponent;
				terms->coefficient = q->coefficient;
			}
			if (q->next != nullptr) {
				terms->next = new Term(0, 0, nullptr);
				terms = terms->next;
			}
		}
		return *this;
	}

	void addTerm(int expon, int coeff) {
		if (term->exponent == 0 && term->coefficient == 0) {
			term->coefficient = coeff;
			term->exponent = expon;
		}
		else {
			Term* q = nullptr;
			Term* p = term;
			if (term->exponent < expon) {
				q = new Term(expon, coeff, nullptr);
				q->next = term;
				term = q;
			}
			else if (term->exponent == expon) {
				term->coefficient += coeff;
				if (term->coefficient == 0) {
					if (term->next == nullptr)
						term->exponent = 0;
					else {
						term = term->next;
						delete p;
					}
				}
			}
			else {
				bool flag = false;
				while (true) {
					if (p->next == nullptr)
						break;
					q = p->next;
					if (q->exponent == expon) {
						flag = true;
						break;
					}
					if (q->exponent < expon)
						break;
					p = p->next;
				}
				if (flag) {
					q->coefficient += coeff;
					if (q->coefficient == 0) {
						p->next = q->next;
						delete q;
					}
				}
				else {
					q = new Term(expon, coeff, nullptr);
					q->next = p->next;
					p->next = q;
				}
			}
		}
	}

	double evaluate(double x) {
		double sum = 0;
		double value;
		Term* p;
		for (p = term; p != nullptr; p = p->next) {
			value = pow(x, p->exponent);
			sum += (p->coefficient * value);
		}
		return sum;
	}

	friend Polynomial operator+(const Polynomial& p, const Polynomial& q) {
		Polynomial result;
		Term* terms_p, * terms_q;
		for (terms_p = p.term; terms_p != nullptr; terms_p = terms_p->next)
			result.addTerm(terms_p->exponent, terms_p->coefficient);
		for (terms_q = q.term; terms_q != nullptr; terms_q = terms_q->next)
			result.addTerm(terms_q->exponent, terms_q->coefficient);
		return result;
	}

	friend Polynomial operator*(const Polynomial& p, const Polynomial& q) {
		Polynomial result;
		Term* terms_p, * terms_q;
		for (terms_p = p.term; terms_p != nullptr; terms_p = terms_p->next) {
			for (terms_q = q.term; terms_q != nullptr; terms_q = terms_q->next) {
				result.addTerm(terms_q->exponent + terms_p->exponent, terms_q->coefficient * terms_p->coefficient);
			}
		}
		return result;
	}

	friend ostream& operator<<(ostream& out, const Polynomial& p) {
		bool flag = true;
		if (p.term->coefficient == 0)
			out << "0";
		else {
			for (Term* q = p.term; q != nullptr; q = q->next) {
				if (flag) {
					if (q->coefficient < 0)
						out << "- ";
				}
				else {
					if (q->coefficient > 0)
						out << "+ ";
					else if (q->coefficient < 0)
						out << "- ";
				}
				if (q->coefficient == 0)
					continue;
				else if (abs(q->coefficient) == 1) {
					if (q->exponent == 0)
						out << abs(q->coefficient);
				}
				else out << abs(q->coefficient);
				if (q->exponent == 0)
					continue;
				else if (q->exponent == 1)
					out << "x";
				else
					out << "x^" << q->exponent;
				if (q->next != nullptr)
					out << " ";
				flag = false;
			}
		}
		return out;
	}

private:
	Term* term;
};

int main()
{
	Polynomial p;
	cout << "Enter number of terms for the first polynomial P(x): " << '\n';
	int nump;
	cin >> nump;
	cout << "Add terms: " << '\n';
	for (int i = 1; i <= nump; i++) {
		int a, b;
		cin >> a >> b;
		p.addTerm(a, b);
	}

	Polynomial q;
	cout << "Enter number of terms for the second polynomial Q(x): " << '\n';
	int numc;
	cin >> numc;
	cout << "Add terms: " << '\n';
	for (int i = 0; i < numc; i++) {
		int c, d;
		cin >> c >> d;
		q.addTerm(c, d);
	}

	cout << "P(x) = " << p << '\n';
	cout << "P(1) = " << p.evaluate(1) << '\n';
	cout << "Q(x) = " << q << '\n';
	cout << "Q(1) = " << q.evaluate(1) << '\n';
	cout << "(P+Q)(x) = " << p + q << '\n';
	cout << "(P*Q)(x) = " << p * q << '\n';
}
