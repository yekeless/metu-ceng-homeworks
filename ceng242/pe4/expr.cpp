#include "expr.h"

namespace sym
{
	Expr::Expr(const Const &c)
	{
		expr_ = c.eval();
	}
	Expr::Expr(const Var &v)
	{
		expr_ = v.eval();
	}
	Expr::Expr(const __expr_t *e) { expr_ = e; }
	Expr::Expr(const __expr_t &e) { expr_ = &e; }
	Expr::Expr(const Expr &e) { expr_ = e.eval(); }

	Expr::~Expr() { delete expr_; }

	__expr_t *Expr::eval(const var_map_t &var_map) const { return expr_->eval(); }
	__expr_t *Expr::diff(const std::string &v) const { return expr_->diff(v); }
	std::ostream &Expr::operator<<(std::ostream &out) const
	{
		out << *expr_;
		return out;
	}
	bool Expr::operator==(const Expr &other) const
	{
		return (*other.expr_) == *expr_;
	}
	bool Expr::operator==(const __expr_t &other) const
	{
		const Expr *temp = dynamic_cast<const Expr *>(&other);
		if (temp)
		{
			return *expr_ == *(temp->expr_);
		}
		return false;
	}
}
