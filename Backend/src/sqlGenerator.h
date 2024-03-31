#pragma once

#include <vector>
#include <string>
#include <stdexcept>

// All code is courtesy and licensed under the MIT license and can be found at https://github.com/six-ddc/sql-builder.git

//MIT License
//
//Copyright(c) 2018 dongcheng.du
//
//Permission is hereby granted, free of charge, to any person obtaining a copy
//of this softwareand associated documentation files(the "Software"), to deal
//in the Software without restriction, including without limitation the rights
//to use, copy, modify, merge, publish, distribute, sublicense, and /or sell
//copies of the Software, and to permit persons to whom the Software is
//furnished to do so, subject to the following conditions :
//
//The above copyright noticeand this permission notice shall be included in all
//copies or substantial portions of the Software.
//
//THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
//IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
//FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.IN NO EVENT SHALL THE
//AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
//LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
//OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
//SOFTWARE.

namespace sqlGenerator {

    class column;

    class Param
    {
    public:
        Param(const std::string& param) : _param(param) {}
        Param(const char* param) : _param(param) {}

    public:
        std::string operator()() const { return param(); }
        inline std::string param() const { return _param; }

    private:
        const std::string _param;
    };

    template <typename T>
    inline std::string to_value(const T& data) {
        return std::to_string(data);
    }

    template <size_t N>
    inline std::string to_value(char const(&data)[N]) {
        std::string str("'");
        str.append(data);
        str.append("'");
        return str;
    }

    template <>
    inline std::string to_value<std::string>(const std::string& data) {
        std::string str("'");
        str.append(data);
        str.append("'");
        return str;
    }

    template <>
    inline std::string to_value<const char*>(const char* const& data) {
        std::string str("'");
        str.append(data);
        str.append("'");
        return str;
    }

    template <>
    inline std::string to_value<Param>(const Param& data) {
        return data();
    }

    template <>
    inline std::string to_value<column>(const column& data);

    /*
    template <>
    static std::string sql::to_value<time_t>(const time_t& data) {
        char buff[128] = {0};
        struct tm* ttime = localtime(&data);
        strftime(buff, sizeof(buff), "%Y-%m-%d %H:%M:%S", ttime);
        std::string str("'");
        str.append(buff);
        str.append("'");
        return str;
    }
    */

    template <typename T>
    void join_vector(std::string& result, const std::vector<T>& vec, const char* sep) {
        size_t size = vec.size();
        for (size_t i = 0; i < size; ++i) {
            if (i < size - 1) {
                result.append(vec[i]);
                result.append(sep);
            }
            else {
                result.append(vec[i]);
            }
        }
    }

    class column
    {
    public:
        column(const std::string& column) {
            _cond = column;
        }
        virtual ~column() {}

        column& as(const std::string& s) {
            _cond.append(" as ");
            _cond.append(s);
            return *this;
        }

        column& is_null() {
            _cond.append(" is null");
            return *this;
        }

        column& is_not_null() {
            _cond.append(" is not null");
            return *this;
        }

        template <typename T>
        column& in(const std::vector<T>& args) {
            size_t size = args.size();
            if (size == 1) {
                _cond.append(" = ");
                _cond.append(to_value(args[0]));
            }
            else {
                _cond.append(" in (");
                for (size_t i = 0; i < size; ++i) {
                    if (i < size - 1) {
                        _cond.append(to_value(args[i]));
                        _cond.append(", ");
                    }
                    else {
                        _cond.append(to_value(args[i]));
                    }
                }
                _cond.append(")");
            }
            return *this;
        }

        template <typename T>
        column& not_in(const std::vector<T>& args) {
            size_t size = args.size();
            if (size == 1) {
                _cond.append(" != ");
                _cond.append(to_value(args[0]));
            }
            else {
                _cond.append(" not in (");
                for (size_t i = 0; i < size; ++i) {
                    if (i < size - 1) {
                        _cond.append(to_value(args[i]));
                        _cond.append(", ");
                    }
                    else {
                        _cond.append(to_value(args[i]));
                    }
                }
                _cond.append(")");
            }
            return *this;
        }

        column& operator &&(column& condition) {
            std::string str("(");
            str.append(_cond);
            str.append(") and (");
            str.append(condition._cond);
            str.append(")");
            condition._cond = str;
            return condition;
        }

        column& operator ||(column& condition) {
            std::string str("(");
            str.append(_cond);
            str.append(") or (");
            str.append(condition._cond);
            str.append(")");
            condition._cond = str;
            return condition;
        }

        column& operator &&(const std::string& condition) {
            _cond.append(" and ");
            _cond.append(condition);
            return *this;
        }

        column& operator ||(const std::string& condition) {
            _cond.append(" or ");
            _cond.append(condition);
            return *this;
        }

        column& operator &&(const char* condition) {
            _cond.append(" and ");
            _cond.append(condition);
            return *this;
        }

        column& operator ||(const char* condition) {
            _cond.append(" or ");
            _cond.append(condition);
            return *this;
        }

        template <typename T>
        column& operator ==(const T& data) {
            _cond.append(" = ");
            _cond.append(to_value(data));
            return *this;
        }

        template <typename T>
        column& operator !=(const T& data) {
            _cond.append(" != ");
            _cond.append(to_value(data));
            return *this;
        }

        template <typename T>
        column& operator >=(const T& data) {
            _cond.append(" >= ");
            _cond.append(to_value(data));
            return *this;
        }

        template <typename T>
        column& operator <=(const T& data) {
            _cond.append(" <= ");
            _cond.append(to_value(data));
            return *this;
        }

        template <typename T>
        column& operator >(const T& data) {
            _cond.append(" > ");
            _cond.append(to_value(data));
            return *this;
        }

        template <typename T>
        column& operator <(const T& data) {
            _cond.append(" < ");
            _cond.append(to_value(data));
            return *this;
        }

        const std::string& str() const {
            return _cond;
        }

        operator bool() {
            return true;
        }
    private:
        std::string _cond;
    };

    template <>
    inline std::string to_value<column>(const column& data) {
        return data.str();
    }


    class SqlModel
    {
    public:
        SqlModel() {}
        virtual ~SqlModel() {}

        virtual const std::string& str() = 0;
        const std::string& last_sql() {
            return _sql;
        }
    private:
        SqlModel(const SqlModel& m) = delete;
        SqlModel& operator =(const SqlModel& data) = delete;
    protected:
        std::string _sql;
    };



    // My Code

    class CreateModel : public SqlModel
    {
    public:
        CreateModel() {}
        virtual ~CreateModel() {}

        CreateModel& columns(const std::string& c, const std::string& type, 
                             const bool& isPrimary = false, 
                             const bool& noNull = false,
                             const bool& unique = false) 
        {
            _columns.push_back(c);
            _types.push_back(type);
            _primary.push_back(isPrimary);
            _nullCondition.push_back(isPrimary ? true : noNull);
            _uniqueCondition.push_back(unique);

            return *this;
        }

        CreateModel& operator()(const std::string& c, const std::string& type,
                                const bool& isPrimary = false,
                                const bool& noNull = false,
                                const bool& unique = false) 
        {
            return columns(c, type, isPrimary, noNull, unique);
        }

        CreateModel& tableName(const std::string& table_name) {
            _table_name = table_name;
            return *this;
        }

        virtual const std::string& str() override {
            _sql.clear();

            _sql.append("create table ");

            _sql.append(_table_name);
            _sql.append("(");
            size_t size = _columns.size();
            std::vector<std::string> primaries;
            for (size_t i = 0; i < size; ++i) {
                std::string line;
                line.append(_columns[i] + " ");
                line.append(_types[i]);
                if (_primary[i]) {
                    primaries.emplace_back(_columns[i]);
                }
                if (_nullCondition[i]) {
                    line.append(" not null");
                }
                if (_uniqueCondition[i] && !_primary[i]) {
                    line.append(" unique");
                }
                _sql.append(line);
                if (i < size - 1) {
                    _sql.append(", ");
                }
            }
            if (!primaries.empty()) {
                _sql.append(", primary key(");
                for (size_t i = 0; i < primaries.size(); i++) {
                    _sql.append(primaries[i]);
                    if (i < primaries.size() - 1) {
                        _sql.append(", ");
                    }
                }
                _sql.append(")");
            }

            _sql.append(")");
            return _sql;
        }

        CreateModel& reset() {
            _table_name.clear();
            _columns.clear();
            _types.clear();
            _primary.clear();
            _nullCondition.clear();
            _foriegnKeys.clear();
            return *this;
        }

        friend inline std::ostream& operator<< (std::ostream& out, CreateModel& mod) {
            out << mod.str();
            return out;
        }

    protected:
        std::string _table_name;
        std::vector<std::string> _columns;
        std::vector<std::string> _types;
        std::vector<bool> _primary;
        std::vector<bool> _nullCondition;
        std::vector<bool> _uniqueCondition;
        std::vector<std::tuple<bool, std::string, std::string>> _foriegnKeys;
    };


    class DropModel : public SqlModel
    {
    public:
        DropModel() {}
        virtual ~DropModel() {}

        DropModel& tableName(const std::string& table_name) {
            _table_name = table_name;
            return *this;
        }

        DropModel& operator()(const std::string& tableName){
            return this->tableName(tableName);
        }

        virtual const std::string& str() override {
            _sql.clear();
            _sql.append("drop table ");
            _sql.append(_table_name);
            return _sql;
        }

        DropModel& reset() {
            _table_name.clear();
            return *this;
        }

        friend inline std::ostream& operator<< (std::ostream& out, DropModel& mod) {
            out << mod.str();
            return out;
        }

    protected:
        std::string _table_name;
    };

    // End My code


    class SelectModel : public SqlModel
    {
    public:
        SelectModel() : _distinct(false) {}
        virtual ~SelectModel() {}

        template <typename... Args>
        SelectModel& select(const std::string& str, Args&&... columns) {
            _select_columns.push_back(str);
            select(columns...);
            return *this;
        }

        // for recursion
        SelectModel& select() {
            return *this;
        }

        SelectModel& distinct() {
            _distinct = true;
            return *this;
        }

        template <typename... Args>
        SelectModel& from(const std::string& table_name, Args&&... tables) {
            if (_table_name.empty()) {
                _table_name = table_name;
            }
            else {
                _table_name.append(", ");
                _table_name.append(table_name);
            }
            from(tables...);
            return *this;
        }

        // for recursion
        SelectModel& from() {
            return *this;
        }

        SelectModel& join(const std::string& table_name) {
            _join_type = "join";
            _join_table = table_name;
            return *this;
        }

        SelectModel& left_join(const std::string& table_name) {
            _join_type = "left join";
            _join_table = table_name;
            return *this;
        }

        SelectModel& left_outer_join(const std::string& table_name) {
            _join_type = "left outer join";
            _join_table = table_name;
            return *this;
        }

        SelectModel& right_join(const std::string& table_name) {
            _join_type = "right join";
            _join_table = table_name;
            return *this;
        }

        SelectModel& right_outer_join(const std::string& table_name) {
            _join_type = "right outer join";
            _join_table = table_name;
            return *this;
        }

        SelectModel& full_join(const std::string& table_name) {
            _join_type = "full join";
            _join_table = table_name;
            return *this;
        }

        SelectModel& full_outer_join(const std::string& table_name) {
            _join_type = "full outer join";
            _join_table = table_name;
            return *this;
        }

        SelectModel& on(const std::string& condition) {
            _join_on_condition.push_back(condition);
            return *this;
        }

        SelectModel& on(const column& condition) {
            _join_on_condition.push_back(condition.str());
            return *this;
        }

        SelectModel& where(const std::string& condition) {
            _where_condition.push_back(condition);
            return *this;
        }

        SelectModel& where(const column& condition) {
            _where_condition.push_back(condition.str());
            return *this;
        }

        template <typename... Args>
        SelectModel& group_by(const std::string& str, Args&&...columns) {
            _groupby_columns.push_back(str);
            group_by(columns...);
            return *this;
        }

        // for recursion
        SelectModel& group_by() {
            return *this;
        }

        SelectModel& having(const std::string& condition) {
            _having_condition.push_back(condition);
            return *this;
        }

        SelectModel& having(const column& condition) {
            _having_condition.push_back(condition.str());
            return *this;
        }

        SelectModel& order_by(const std::string& order_by) {
            _order_by = order_by;
            return *this;
        }

        template <typename T>
        SelectModel& limit(const T& limit) {
            _limit = std::to_string(limit);
            return *this;
        }
        template <typename T>
        SelectModel& limit(const T& offset, const T& limit) {
            _offset = std::to_string(offset);
            _limit = std::to_string(limit);
            return *this;
        }
        template <typename T>
        SelectModel& offset(const T& offset) {
            _offset = std::to_string(offset);
            return *this;
        }

        virtual const std::string& str() override {
            _sql.clear();
            _sql.append("select ");
            if (_distinct) {
                _sql.append("distinct ");
            }
            join_vector(_sql, _select_columns, ", ");
            _sql.append(" from ");
            _sql.append(_table_name);
            if (!_join_type.empty()) {
                _sql.append(" ");
                _sql.append(_join_type);
                _sql.append(" ");
                _sql.append(_join_table);
            }
            if (!_join_on_condition.empty()) {
                _sql.append(" on ");
                join_vector(_sql, _join_on_condition, " and ");
            }
            if (!_where_condition.empty()) {
                _sql.append(" where ");
                join_vector(_sql, _where_condition, " and ");
            }
            if (!_groupby_columns.empty()) {
                _sql.append(" group by ");
                join_vector(_sql, _groupby_columns, ", ");
            }
            if (!_having_condition.empty()) {
                _sql.append(" having ");
                join_vector(_sql, _having_condition, " and ");
            }
            if (!_order_by.empty()) {
                _sql.append(" order by ");
                _sql.append(_order_by);
            }
            if (!_limit.empty()) {
                _sql.append(" limit ");
                _sql.append(_limit);
            }
            if (!_offset.empty()) {
                _sql.append(" offset ");
                _sql.append(_offset);
            }
            return _sql;
        }

        SelectModel& reset() {
            _select_columns.clear();
            _distinct = false;
            _groupby_columns.clear();
            _table_name.clear();
            _join_type.clear();
            _join_table.clear();
            _join_on_condition.clear();
            _where_condition.clear();
            _having_condition.clear();
            _order_by.clear();
            _limit.clear();
            _offset.clear();
            return *this;
        }
        friend inline std::ostream& operator<< (std::ostream& out, SelectModel& mod) {
            out << mod.str();
            return out;
        }

    protected:
        std::vector<std::string> _select_columns;
        bool _distinct;
        std::vector<std::string> _groupby_columns;
        std::string _table_name;
        std::string _join_type;
        std::string _join_table;
        std::vector<std::string> _join_on_condition;
        std::vector<std::string> _where_condition;
        std::vector<std::string> _having_condition;
        std::string _order_by;
        std::string _limit;
        std::string _offset;
    };



    class InsertModel : public SqlModel
    {
    public:
        InsertModel() {}
        virtual ~InsertModel() {}

        template <typename T>
        InsertModel& insert(const std::string& c, const T& data) {
            _columns.push_back(c);
            _values.push_back(to_value(data));
            return *this;
        }

        template <typename T>
        InsertModel& operator()(const std::string& c, const T& data) {
            return insert(c, data);
        }

        InsertModel& into(const std::string& table_name) {
            _table_name = table_name;
            return *this;
        }

        InsertModel& replace(bool var) {
            _replace = var;
            return *this;
        }

        virtual const std::string& str() override {
            _sql.clear();
            std::string v_ss;

            if (_replace) {
                _sql.append("insert or replace into ");
            }
            else {
                _sql.append("insert into ");
            }

            _sql.append(_table_name);
            _sql.append("(");
            v_ss.append(" values(");
            size_t size = _columns.size();
            for (size_t i = 0; i < size; ++i) {
                if (i < size - 1) {
                    _sql.append(_columns[i]);
                    _sql.append(", ");
                    v_ss.append(_values[i]);
                    v_ss.append(", ");
                }
                else {
                    _sql.append(_columns[i]);
                    _sql.append(")");
                    v_ss.append(_values[i]);
                    v_ss.append(")");
                }
            }
            _sql.append(v_ss);
            return _sql;
        }

        InsertModel& reset() {
            _table_name.clear();
            _columns.clear();
            _values.clear();
            return *this;
        }

        friend inline std::ostream& operator<< (std::ostream& out, InsertModel& mod) {
            out << mod.str();
            return out;
        }

    protected:
        bool _replace = false;
        std::string _table_name;
        std::vector<std::string> _columns;
        std::vector<std::string> _values;
    };


    template <>
    inline InsertModel& InsertModel::insert(const std::string& c, const std::nullptr_t&) {
        _columns.push_back(c);
        _values.push_back("null");
        return *this;
    }


    class UpdateModel : public SqlModel
    {
    public:
        UpdateModel() {}
        virtual ~UpdateModel() {}

        UpdateModel& update(const std::string& table_name) {
            _table_name = table_name;
            return *this;
        }

        template <typename T>
        UpdateModel& set(const std::string& c, const T& data) {
            std::string str(c);
            str.append(" = ");
            str.append(to_value(data));
            _set_columns.push_back(str);
            return *this;
        }

        template <typename T>
        UpdateModel& operator()(const std::string& c, const T& data) {
            return set(c, data);
        }

        UpdateModel& where(const std::string& condition) {
            _where_condition.push_back(condition);
            return *this;
        }

        UpdateModel& where(const column& condition) {
            _where_condition.push_back(condition.str());
            return *this;
        }

        virtual const std::string& str() override {
            _sql.clear();
            _sql.append("update ");
            _sql.append(_table_name);
            _sql.append(" set ");
            join_vector(_sql, _set_columns, ", ");
            size_t size = _where_condition.size();
            if (size > 0) {
                _sql.append(" where ");
                join_vector(_sql, _where_condition, " and ");
            }
            return _sql;
        }

        UpdateModel& reset() {
            _table_name.clear();
            _set_columns.clear();
            _where_condition.clear();
            return *this;
        }
        friend inline std::ostream& operator<< (std::ostream& out, UpdateModel& mod) {
            out << mod.str();
            return out;
        }

    protected:
        std::vector<std::string> _set_columns;
        std::string _table_name;
        std::vector<std::string> _where_condition;
    };

    template <>
    inline UpdateModel& UpdateModel::set(const std::string& c, const std::nullptr_t&) {
        std::string str(c);
        str.append(" = null");
        _set_columns.push_back(str);
        return *this;
    }


    class DeleteModel : public SqlModel
    {
    public:
        DeleteModel() {}
        virtual ~DeleteModel() {}

        DeleteModel& _delete() {
            return *this;
        }

        template <typename... Args>
        DeleteModel& from(const std::string& table_name, Args&&... tables) {
            if (_table_name.empty()) {
                _table_name = table_name;
            }
            else {
                _table_name.append(", ");
                _table_name.append(table_name);
            }
            from(tables...);
            return *this;
        }

        // for recursion
        DeleteModel& from() {
            return *this;
        }

        DeleteModel& where(const std::string& condition) {
            _where_condition.push_back(condition);
            return *this;
        }

        DeleteModel& where(const column& condition) {
            _where_condition.push_back(condition.str());
            return *this;
        }

        virtual const std::string& str() override {
            _sql.clear();
            _sql.append("delete from ");
            _sql.append(_table_name);
            size_t size = _where_condition.size();
            if (size > 0) {
                _sql.append(" where ");
                join_vector(_sql, _where_condition, " and ");
            }
            return _sql;
        }

        DeleteModel& reset() {
            _table_name.clear();
            _where_condition.clear();
            return *this;
        }
        friend inline std::ostream& operator<< (std::ostream& out, DeleteModel& mod) {
            out << mod.str();
            return out;
        }

    protected:
        std::string _table_name;
        std::vector<std::string> _where_condition;
    };

}

