#include "binary.h"
#include "nullary.h"
#include <math.h>

namespace sym
{
	bool AddOp::is_add() const { return true; }

	__expr_t *AddOp::eval(const var_map_t &vars) const
	{
		__expr_t *LHS = lhs_->eval(vars);
		__expr_t *RHS = rhs_->eval(vars);
		const Const *L = dynamic_cast<Const *>(LHS);
		const Const *R = dynamic_cast<Const *>(RHS);
		if (L && R)
		{
			double val = dynamic_cast<Const *>(LHS)->get_value() + dynamic_cast<Const *>(RHS)->get_value();
			delete LHS;
			delete RHS;
			return new Const(val);
		}
		if (L && dynamic_cast<Const *>(LHS)->get_value() == 0.0)
		{
			delete LHS;
			return RHS;
		}
		if (R && dynamic_cast<Const *>(RHS)->get_value() == 0.0)
		{
			delete RHS;
			return LHS;
		}

		return new AddOp(LHS, RHS);
	}

	__expr_t *AddOp::diff(const std::string &v) const
	{
		__expr_t *derivative_left = lhs_->diff(v);
		__expr_t *derivative_right = rhs_->diff(v);
		return AddOp(derivative_left, derivative_right).eval();
	}

	std::ostream &AddOp::operator<<(std::ostream &out) const
	{
		if (lhs_->is_nullary())
		{
			out << *lhs_;
		}
		else
		{
			out << "(" << *lhs_ << ")";
		}
		out << " + ";
		if (rhs_->is_nullary())
		{
			out << *rhs_;
		}
		else
		{
			out << "(" << *rhs_ << ")";
		}
		return out;
	}

	bool AddOp::operator==(const __expr_t &other_) const
	{

		const AddOp *new_add = dynamic_cast<const AddOp *>(&other_);
		if (new_add != nullptr)
		{
			return ((lhs_ == new_add->lhs_) && (rhs_ == new_add->rhs_)) || ((lhs_ == new_add->rhs_) && (rhs_ == new_add->lhs_));
		}
		return false;
	}
}

namespace sym
{
	bool MulOp::is_mul() const { return true; }

	__expr_t *MulOp::eval(const var_map_t &vars) const
	{
		__expr_t *LHS = lhs_->eval(vars);
		__expr_t *RHS = rhs_->eval(vars);
		const Const *L = dynamic_cast<Const *>(LHS);
		const Const *R = dynamic_cast<Const *>(RHS);
		if ((L != nullptr && dynamic_cast<Const *>(LHS)->get_value() == 0.0) || (R != nullptr && dynamic_cast<Const *>(RHS)->get_value() == 0.0))
		{
			delete RHS;
			delete LHS;
			return new Const(0.0);
		}
		if ((R != nullptr && dynamic_cast<Const *>(RHS)->get_value() == 1.0))
		{
			delete RHS;
			return LHS;
		}
		if (L != nullptr && dynamic_cast<Const *>(LHS)->get_value() == 1.0)
		{
			delete LHS;
			return RHS;
		}
		if (L != nullptr && R != nullptr)
		{
			double val = L->get_value() * R->get_value();
			delete LHS;
			delete RHS;
			return new Const(val);
		}
		return new MulOp(LHS, RHS);
	}

	__expr_t *MulOp::diff(const std::string &v) const
	{
		__expr_t *left_derivative = lhs_->diff(v);
		__expr_t *right_derivative = rhs_->diff(v);

		__expr_t *result = new AddOp(new MulOp(left_derivative, rhs_), new MulOp(lhs_, right_derivative));
		return result->eval();
	}

	std::ostream &MulOp::operator<<(std::ostream &out) const
	{

		if (lhs_->is_nullary())
		{
			out << *lhs_;
		}
		else
		{
			out << "(" << *lhs_ << ")";
		}
		out << " * ";
		if (rhs_->is_nullary())
		{
			out << *rhs_;
		}
		else
		{
			out << "(" << *rhs_ << ")";
		}
		return out;
	}

	bool MulOp::operator==(const __expr_t &other_) const
	{
		const MulOp *new_mul = dynamic_cast<const MulOp *>(&other_);
		if (new_mul != nullptr)
		{
			return ((lhs_ == new_mul->lhs_) && (rhs_ == new_mul->rhs_)) || ((lhs_ == new_mul->rhs_) && (rhs_ == new_mul->lhs_));
		}
		return false;
	}
}
