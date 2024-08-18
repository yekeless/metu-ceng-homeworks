package ceng.ceng351.cengfactorydb;

import java.sql.*;
import java.util.*;


public class CENGFACTORYDB implements ICENGFACTORYDB{
    /**
     * Place your initialization code inside if required.
     *
     * <p>
     * This function will be called before all other operations. If your implementation
     * need initialization , necessary operations should be done inside this function. For
     * example, you can set your connection to the database server inside this function.
     */
    String user = "e2521722"; // TODO: Your userName
    String password = "GfBW4Pj8f80q"; //  TODO: Your password
    String host = "144.122.71.128"; // host name
    String database = "db2521722"; // TODO: Your database name
    int port = 8080; // port
    Connection connection = null;
    public void initialize() {
        String url = "jdbc:mysql://" + host + ":" + port + "/" + database;

        try {
            Class.forName("com.mysql.cj.jdbc.Driver");
            connection =  DriverManager.getConnection(url, user, password);
        }
        catch (SQLException | ClassNotFoundException e) {
            e.printStackTrace();
        }
    }

    /**
     * Should create the necessary tables when called.
     *
     * @return the number of tables that are created successfully.
     */
    public int createTables() {
        int count=0;
        String queryFactory="CREATE TABLE IF NOT EXISTS Factory(factoryId INT NOT NULL,factoryName Text,factoryType Text,country Text, PRIMARY KEY(factoryId))";
        String queryEmployee="CREATE TABLE IF NOT EXISTS Employee(employeeId INT NOT NULL,employeeName Text,department Text,salary INT,PRIMARY KEY(employeeId))";
        String queryWorks="CREATE TABLE IF NOT EXISTS Works_In(factoryId INT NOT NULL,employeeId INT NOT NULL,startDate Date,PRIMARY KEY(factoryId,employeeId),FOREIGN KEY (factoryId) REFERENCES Factory(factoryId) ON DELETE CASCADE ON UPDATE CASCADE,FOREIGN KEY (employeeId) REFERENCES Employee(employeeId) ON DELETE CASCADE ON UPDATE CASCADE)";
        String queryProduct="CREATE TABLE IF NOT EXISTS Product(productId INT NOT NULL,productName Text,productType Text,PRIMARY KEY (productId))";
        String queryProduce="CREATE TABLE IF NOT EXISTS Produce(factoryId INT NOT NULL,productId INT NOT NULL,amount INT,productionCost INT,PRIMARY KEY(factoryId,productId),FOREIGN KEY (factoryId) REFERENCES Factory(factoryId) ON DELETE CASCADE ON UPDATE CASCADE,FOREIGN KEY (productId) REFERENCES Product(productId) ON DELETE CASCADE ON UPDATE CASCADE)";
        String queryShipment="CREATE TABLE IF NOT EXISTS Shipment(factoryId INT NOT NULL,productId INT NOT NULL,amount INT,pricePerUnit INT,PRIMARY KEY(factoryId,productId),FOREIGN KEY (factoryId) REFERENCES Factory(factoryId) ON DELETE CASCADE ON UPDATE CASCADE,FOREIGN KEY (productId) REFERENCES Product(productId) ON DELETE CASCADE ON UPDATE CASCADE)";
        try{
            Statement query= connection.createStatement();
            query.executeUpdate(queryFactory);count++;
            query.executeUpdate(queryEmployee);count++;
            query.executeUpdate(queryWorks);count++;
            query.executeUpdate(queryProduct);count++;
            query.executeUpdate(queryProduce);count++;
            query.executeUpdate(queryShipment);count++;
        }
        catch(SQLException e){
            e.printStackTrace();
        }
        return count;
    }

    /**
     * Should drop the tables if exists when called.
     *
     * @return the number of tables are dropped successfully.
     */
    public int dropTables() {
        int count=0;
        String DropFactory="DROP TABLE IF EXISTS Factory";
        String DropEmployee="DROP TABLE IF EXISTS Employee";
        String DropWorks="DROP TABLE IF EXISTS Works_In";
        String DropProduct="DROP TABLE IF EXISTS Product";
        String DropProduce="DROP TABLE IF EXISTS Produce";
        String DropeShipment="DROP TABLE IF EXISTS Shipment";
        try{
            Statement query= connection.createStatement();
            query.executeUpdate(DropeShipment);count++;
            query.executeUpdate(DropProduce);count++;
            query.executeUpdate(DropProduct);count++;
            query.executeUpdate(DropWorks);count++;
            query.executeUpdate(DropEmployee);count++;
            query.executeUpdate(DropFactory);count++;
        }
        catch(SQLException e){
            e.printStackTrace();
        }
        return count;
    }

    /**
     * Should insert an array of Factory into the database.
     *
     * @return Number of rows inserted successfully.
     */
    public int insertFactory(Factory[] factories) {
        int numRowsInserted = 0;
        try {
            String insertFactoryQuery = "INSERT INTO Factory (factoryId, factoryName, factoryType, country) VALUES (?, ?, ?, ?)";
            PreparedStatement insertFactoryStatement = connection.prepareStatement(insertFactoryQuery);
            for (Factory factory : factories) {
                insertFactoryStatement.setInt(1, factory.getFactoryId());
                insertFactoryStatement.setString(2, factory.getFactoryName());
                insertFactoryStatement.setString(3, factory.getFactoryType());
                insertFactoryStatement.setString(4, factory.getCountry());
                insertFactoryStatement.executeUpdate();
                numRowsInserted++;
            }
        }
        catch (SQLException e) {
            e.printStackTrace();
        }
        return numRowsInserted;
    }

    /**
     * Should insert an array of Employee into the database.
     *
     * @return Number of rows inserted successfully.
     */
    public int insertEmployee(Employee[] employees) {
        int numRowsInserted = 0;
        try {
            String insertEmployeeQuery = "INSERT INTO Employee (employeeId, employeeName, department, salary) VALUES (?, ?, ?, ?)";
            PreparedStatement insertEmployeeStatement = connection.prepareStatement(insertEmployeeQuery);
            for (Employee employee : employees) {
                insertEmployeeStatement.setInt(1, employee.getEmployeeId());
                insertEmployeeStatement.setString(2, employee.getEmployeeName());
                insertEmployeeStatement.setString(3, employee.getDepartment());
                insertEmployeeStatement.setInt(4, employee.getSalary());
                insertEmployeeStatement.executeUpdate();
                numRowsInserted++;
            }
        }
        catch (SQLException e) {
            e.printStackTrace();
        }
        return numRowsInserted;
    }

    /**
     * Should insert an array of WorksIn into the database.
     *
     * @return Number of rows inserted successfully.
     */
    public int insertWorksIn(WorksIn[] worksIns) {
        int numRowsInserted = 0;
        try {
            String insertWorksInQuery = "INSERT INTO Works_In (factoryId, employeeId, startDate) VALUES (?, ?, ?)";
            PreparedStatement insertWorksInStatement = connection.prepareStatement(insertWorksInQuery);
            for (WorksIn worksIn : worksIns) {
                insertWorksInStatement.setInt(1, worksIn.getFactoryId());
                insertWorksInStatement.setInt(2, worksIn.getEmployeeId());
                insertWorksInStatement.setString(3, worksIn.getStartDate());
                insertWorksInStatement.executeUpdate();
                numRowsInserted++;
            }
        }
        catch (SQLException e) {
            e.printStackTrace();
        }
        return numRowsInserted;
    }

    /**
     * Should insert an array of Product into the database.
     *
     * @return Number of rows inserted successfully.
     */
    public int insertProduct(Product[] products) {
        int numRowsInserted = 0;
        try {
            String insertProductQuery = "INSERT INTO Product (productId, productName, productType) VALUES (?, ?, ?)";
            PreparedStatement insertProductStatement = connection.prepareStatement(insertProductQuery);
            for (Product product : products) {
                insertProductStatement.setInt(1, product.getProductId());
                insertProductStatement.setString(2, product.getProductName());
                insertProductStatement.setString(3, product.getProductType());
                insertProductStatement.executeUpdate();
                numRowsInserted++;
            }
        }
        catch (SQLException e) {
            e.printStackTrace();
        }
        return numRowsInserted;
    }


    /**
     * Should insert an array of Produce into the database.
     *
     * @return Number of rows inserted successfully.
     */
    public int insertProduce(Produce[] produces) {
        int numRowsInserted = 0;
        try {
            String insertProduceQuery = "INSERT INTO Produce (factoryId, productId, amount, productionCost) VALUES (?, ?, ?, ?)";
            PreparedStatement insertProduceStatement = connection.prepareStatement(insertProduceQuery);
            for (Produce produce : produces) {
                insertProduceStatement.setInt(1, produce.getFactoryId());
                insertProduceStatement.setInt(2, produce.getProductId());
                insertProduceStatement.setInt(3, produce.getAmount());
                insertProduceStatement.setInt(4, produce.getProductionCost());
                insertProduceStatement.executeUpdate();
                numRowsInserted++;
            }
        }
        catch (SQLException e) {
            e.printStackTrace();
        }
        return numRowsInserted;
    }


    /**
     * Should insert an array of Shipment into the database.
     *
     * @return Number of rows inserted successfully.
     */
    public int insertShipment(Shipment[] shipments) {
        int numRowsInserted = 0;
        try {
            String insertShipmentQuery = "INSERT INTO Shipment (factoryId, productId, amount, pricePerUnit) VALUES (?, ?, ?, ?)";
            PreparedStatement insertShipmentStatement = connection.prepareStatement(insertShipmentQuery);
            for (Shipment shipment : shipments) {
                insertShipmentStatement.setInt(1, shipment.getFactoryId());
                insertShipmentStatement.setInt(2, shipment.getProductId());
                insertShipmentStatement.setInt(3, shipment.getAmount());
                insertShipmentStatement.setInt(4, shipment.getPricePerUnit());
                insertShipmentStatement.executeUpdate();
                numRowsInserted++;
            }
        }
        catch (SQLException e) {
            e.printStackTrace();
        }
        return numRowsInserted;
    }

    /**
     * Should return all factories that are located in a particular country.
     *
     * @return Factory[]
     */
    public Factory[] getFactoriesForGivenCountry(String country) {
        List<Factory> factories = new ArrayList<>();
        try {
            String selectFactoriesQuery = "SELECT factoryId, factoryName, factoryType, country FROM Factory WHERE Factory.country = ? ORDER BY factoryId ASC";
            PreparedStatement selectFactoriesStatement = connection.prepareStatement(selectFactoriesQuery);
            selectFactoriesStatement.setString(1, country);
            ResultSet resultSet = selectFactoriesStatement.executeQuery();
            while (resultSet.next()) {
                int factoryId = resultSet.getInt("factoryId");
                String factoryName = resultSet.getString("factoryName");
                String factoryType = resultSet.getString("factoryType");
                String factoryCountry = resultSet.getString("country");
                Factory factory = new Factory(factoryId, factoryName, factoryType, factoryCountry);
                factories.add(factory);
            }
            resultSet.close();
            selectFactoriesStatement.close();
        }
        catch (SQLException e) {
            e.printStackTrace();
        }
        return factories.toArray(new Factory[0]);
    }



    /**
     * Should return all factories without any working employees.
     *
     * @return Factory[]
     */
    public Factory[] getFactoriesWithoutAnyEmployee() {
        List<Factory> factories = new ArrayList<>();
        try {
            String selectFactoriesQuery = "SELECT factoryId, factoryName, factoryType, country FROM Factory WHERE Factory.factoryId NOT IN (SELECT DISTINCT factoryId FROM Works_In) ORDER BY factoryId ASC";
            PreparedStatement selectFactoriesStatement = connection.prepareStatement(selectFactoriesQuery);
            ResultSet resultSet = selectFactoriesStatement.executeQuery();
            while (resultSet.next()) {
                int factoryId = resultSet.getInt("factoryId");
                String factoryName = resultSet.getString("factoryName");
                String factoryType = resultSet.getString("factoryType");
                String factoryCountry = resultSet.getString("country");
                Factory factory = new Factory(factoryId, factoryName, factoryType, factoryCountry);
                factories.add(factory);
            }
            resultSet.close();
            selectFactoriesStatement.close();
        }
        catch (SQLException e) {
            e.printStackTrace();
        }
        return factories.toArray(new Factory[0]);
    }

    /**
     * Should return all factories that produce all products for a particular productType
     *
     * @return Factory[]
     */
    public Factory[] getFactoriesProducingAllForGivenType(String productType) {
        List<Factory> factories = new ArrayList<>();

        try {
            String selectFactoriesQuery = "SELECT f.factoryId, f.factoryName, f.factoryType, f.country " +
                    "FROM Factory f " +
                    "WHERE NOT EXISTS (SELECT DISTINCT p.productId " +
                    "FROM Product p " +
                    "WHERE p.productType = ? " +
                    "EXCEPT SELECT p1.productId FROM Produce p1 WHERE p1.factoryId=f.factoryId) " +
                    "ORDER BY f.factoryId ASC";
            PreparedStatement selectFactoriesStatement = connection.prepareStatement(selectFactoriesQuery);
            selectFactoriesStatement.setString(1, productType);
            ResultSet resultSet = selectFactoriesStatement.executeQuery();
            while (resultSet.next()) {
                int factoryId = resultSet.getInt("factoryId");
                String factoryName = resultSet.getString("factoryName");
                String factoryType = resultSet.getString("factoryType");
                String factoryCountry = resultSet.getString("country");
                Factory factory = new Factory(factoryId, factoryName, factoryType, factoryCountry);
                factories.add(factory);
            }
            resultSet.close();
            selectFactoriesStatement.close();
        }
        catch (SQLException e) {
            e.printStackTrace();
        }
        return factories.toArray(new Factory[0]);
    }


    /**
     * Should return the products that are produced in a particular factory but
     * don’t have any shipment from that factory.
     *
     * @return Product[]
     */
    public Product[] getProductsProducedNotShipped() {
        List<Product> products = new ArrayList<>();
        try {
            String selectProductsQuery = "SELECT DISTINCT p.productId, p.productName, p.productType " +
                    "FROM Product p ,Produce pr " +
                    "WHERE p.productId=pr.productId AND " +
                    "pr.factoryId NOT IN " +
                    "(SELECT DISTINCT factoryId FROM Shipment s WHERE s.productId=pr.productId) " +
                    "ORDER BY p.productId ASC";
            PreparedStatement selectProductsStatement = connection.prepareStatement(selectProductsQuery);
            ResultSet resultSet = selectProductsStatement.executeQuery();
            while (resultSet.next()) {
                int productId = resultSet.getInt("productId");
                String productName = resultSet.getString("productName");
                String productType = resultSet.getString("productType");
                Product product = new Product(productId, productName, productType);
                products.add(product);
            }
            resultSet.close();
            selectProductsStatement.close();
        }
        catch (SQLException e) {
            e.printStackTrace();
        }
        return products.toArray(new Product[0]);
    }


    /**
     * For a given factoryId and department, should return the average salary of
     *     the employees working in that factory and that specific department.
     *
     * @return double
     */
    public double getAverageSalaryForFactoryDepartment(int factoryId, String department) {
        double averageSalary = 0.0;
        try {
            String selectAverageSalaryQuery = "SELECT DISTINCT AVG(e.salary) AS average_salary " +
                    "FROM Employee e,Works_In w " +
                    "WHERE e.employeeId = w.employeeId AND w.factoryId = ? GROUP BY e.department HAVING e.department = ? ";

            PreparedStatement selectAverageSalaryStatement = connection.prepareStatement(selectAverageSalaryQuery);
            selectAverageSalaryStatement.setInt(1, factoryId);
            selectAverageSalaryStatement.setString(2, department);
            ResultSet resultSet = selectAverageSalaryStatement.executeQuery();
            if (resultSet.next()) {
                averageSalary = resultSet.getDouble("average_salary");
            }
            resultSet.close();
            selectAverageSalaryStatement.close();
        }
        catch (SQLException e) {
            e.printStackTrace();
        }
        return averageSalary;
    }


    /**
     * Should return the most profitable products for each factory
     *
     * @return QueryResult.MostValueableProduct[]
     */

    public QueryResult.MostValueableProduct[] getMostValueableProducts() {
        List<QueryResult.MostValueableProduct> mostValueableProducts = new ArrayList<>();
        try {

            String selectProfitQuery ="WITH ProfitTable AS (" +
                    "    SELECT " +
                    "        pr.factoryId, " +
                    "        p.productId, " +
                    "        p.productName, " +
                    "        p.productType, " +
                    "        COALESCE((SELECT SUM(amount * pricePerUnit) FROM Shipment WHERE factoryId = pr.factoryId AND productId = p.productId), 0) - " +
                    "        COALESCE((SELECT SUM(amount * productionCost) FROM Produce WHERE factoryId = pr.factoryId AND productId = p.productId), 0) AS profit " +
                    "    FROM " +
                    "        Produce pr " +
                    "    INNER JOIN " +
                    "        Product p ON pr.productId = p.productId " +
                    "), " +
                    "RankProfit AS (" +
                    "    SELECT " +
                    "        *, " +
                    "        RANK() OVER (PARTITION BY factoryId ORDER BY profit DESC) as profit_rank " +
                    "    FROM " +
                    "        ProfitTable " +
                    ") " +
                    "SELECT " +
                    "    factoryId, " +
                    "    productId, " +
                    "    productName, " +
                    "    productType, " +
                    "    profit " +
                    "FROM " +
                    "    RankProfit " +
                    "WHERE " +
                    "    profit_rank = 1 " +
                    "ORDER BY " +
                    "    profit DESC, " +
                    "    factoryId ASC;";




            PreparedStatement selectProfitStatement = connection.prepareStatement(selectProfitQuery);
            ResultSet resultSet = selectProfitStatement.executeQuery();

            while (resultSet.next()) {
                int factoryId= resultSet.getInt("factoryId");
                int productId= resultSet.getInt("productId");
                String productName= resultSet.getString("productName");
                String productType=resultSet.getString("productType");
                int Profit= resultSet.getInt("Profit");
                QueryResult.MostValueableProduct NP= new QueryResult.MostValueableProduct(factoryId,productId,productName,productType,Profit);
                mostValueableProducts.add(NP);
            }
            resultSet.close();
            selectProfitStatement.close();
        }
        catch (SQLException e) {
            e.printStackTrace();
        }
        return mostValueableProducts.toArray(new QueryResult.MostValueableProduct[0]);
    }

    /**
     * For each product, return the factories that gather the highest profit
     * for that product
     *
     * @return QueryResult.MostValueableProduct[]
     */

    public QueryResult.MostValueableProduct[] getMostValueableProductsOnFactory() {
        List<QueryResult.MostValueableProduct> result = new ArrayList<>();
        String query="WITH ProfitTable AS (" +
                "    SELECT " +
                "        p.factoryId, " +
                "        p.productId, " +
                "        pt.productName, " +
                "        pt.productType, " +
                "        COALESCE((SELECT SUM(amount * pricePerUnit) FROM Shipment WHERE factoryId = p.factoryId AND productId = p.productId), 0) - " +
                "        COALESCE((SELECT SUM(amount * productionCost) FROM Produce WHERE factoryId = p.factoryId AND productId = p.productId), 0) AS profit " +
                "    FROM " +
                "        Produce p " +
                "    JOIN " +
                "        Product pt ON pt.productId = p.productId " +
                "), " +
                "RankProfit AS (" +
                "    SELECT " +
                "        *, " +
                "        RANK() OVER (PARTITION BY productId ORDER BY profit DESC) as profit_rank " +
                "    FROM " +
                "        ProfitTable " +
                ") " +
                "SELECT " +
                "    factoryId, " +
                "    productId, " +
                "    productName, " +
                "    productType, " +
                "    profit " +
                "FROM " +
                "    RankProfit " +
                "WHERE " +
                "    profit_rank = 1 " +
                "ORDER BY " +
                "    profit DESC, " +
                "    productId ASC;";



        try {
            PreparedStatement statement = connection.prepareStatement(query);
            ResultSet resultSet = statement.executeQuery();
            while (resultSet.next()) {
                int factoryId = resultSet.getInt("factoryId");
                int productId = resultSet.getInt("productId");
                String productName = resultSet.getString("productName");
                String productType = resultSet.getString("productType");
                double profit = resultSet.getDouble("profit");
                QueryResult.MostValueableProduct product = new QueryResult.MostValueableProduct(factoryId, productId, productName, productType, profit);
                result.add(product);
            }
            resultSet.close();
            statement.close();
        }
        catch (SQLException e) {
            e.printStackTrace();
        }
        return result.toArray(new QueryResult.MostValueableProduct[0]);
    }


    /**
     * For each department, should return all employees that are paid under the
     *     average salary for that department. You consider the employees
     *     that do not work earning ”0”.
     *
     * @return QueryResult.LowSalaryEmployees[]
     */
    public QueryResult.LowSalaryEmployees[] getLowSalaryEmployeesForDepartments() {
        List<QueryResult.LowSalaryEmployees> result = new ArrayList<>();
        
        String query =
                "WITH EmployeeSalary AS (" +
                        "    SELECT " +
                        "        e.employeeId, " +
                        "        e.employeeName, " +
                        "        e.department, " +
                        "        CASE WHEN wi.employeeId IS NULL THEN 0 ELSE e.salary END AS salary " +
                        "    FROM " +
                        "        Employee e " +
                        "    LEFT JOIN " +
                        "        Works_In wi ON e.employeeId = wi.employeeId " +
                        "), " +
                        "DepartmentAvgSalary AS (" +
                        "    SELECT " +
                        "        department, " +
                        "        AVG(salary) AS avg " +
                        "    FROM " +
                        "        EmployeeSalary " +
                        "    GROUP BY " +
                        "        department " +
                        ") " +
                        "SELECT DISTINCT " +
                        "    es.employeeId, " +
                        "    es.employeeName, " +
                        "    es.department, " +
                        "    es.salary " +
                        "FROM " +
                        "    EmployeeSalary es " +
                        "JOIN " +
                        "    DepartmentAvgSalary das ON es.department = das.department " +
                        "WHERE " +
                        "    es.salary <= das.avg OR es.salary = 0 " +
                        "ORDER BY " +
                        "    es.employeeId ASC;";
        try{
            PreparedStatement LowSalaryQuery=connection.prepareStatement(query);
            ResultSet resultSet=LowSalaryQuery.executeQuery();
            while(resultSet.next()){
                int employeeId=resultSet.getInt("employeeId");
                String employeeName= resultSet.getString("employeeName");
                String department= resultSet.getString("department");
                int salary= resultSet.getInt("salary");
                QueryResult.LowSalaryEmployees employee =new QueryResult.LowSalaryEmployees(employeeId,employeeName,department,salary);
                result.add(employee);
            }
        }
        catch (SQLException e){
            e.printStackTrace();
        }

        return result.toArray(new QueryResult.LowSalaryEmployees[0]);
    }


    /**
     * For the products of given productType, increase the productionCost of every unit by a given percentage.
     *
     * @return number of rows affected
     */
    public int increaseCost(String productType, double percentage) {
        int count=0;
        String increasedProducts = "UPDATE Product JOIN Produce ON Produce.productId = Product.productId SET Produce.productionCost = Produce.productionCost + (Produce.productionCost * ?) WHERE Product.productType = ? ";
        try {
            PreparedStatement query = connection.prepareStatement(increasedProducts);
            query.setDouble(1, percentage);
            query.setString(2, productType);
            count = query.executeUpdate();
        }
        catch(SQLException e){
            e.printStackTrace();
        }
        return count;
    }


    /**
     * Deleting all employees that have not worked since the given date.
     *
     * @return number of rows affected
     */
    public int deleteNotWorkingEmployees(String givenDate) {
        int count=0;
        String deleteEmployee= "DELETE FROM Employee WHERE employeeId NOT IN (SELECT employeeId FROM Works_In WHERE startDate >= ?)";
        try {
            PreparedStatement query = connection.prepareStatement(deleteEmployee);
            query.setString(1, givenDate);
            count = query.executeUpdate();
        }
        catch(SQLException e){
            e.printStackTrace();
        }
        return count;
    }


    /**
     * *******************
     * *******************
     * *******************
     * *******************
     *  THE METHODS AFTER THIS LINE WILL NOT BE GRADED.
     *  YOU DON'T HAVE TO SOLVE THEM, LEAVE THEM AS IS IF YOU WANT.
     *  IF YOU HAVE ANY QUESTIONS, REACH ME VIA EMAIL.
     * *******************
     * *******************
     * *******************
     * *******************
     */

    /**
     * For each department, find the rank of the employees in terms of
     * salary. Peers are considered tied and receive the same rank. After
     * that, there should be a gap.
     *
     * @return QueryResult.EmployeeRank[]
     */
    public QueryResult.EmployeeRank[] calculateRank() {
        return new QueryResult.EmployeeRank[0];
    }

    /**
     * For each department, find the rank of the employees in terms of
     * salary. Everything is the same but after ties, there should be no
     * gap.
     *
     * @return QueryResult.EmployeeRank[]
     */
    public QueryResult.EmployeeRank[] calculateRank2() {
        return new QueryResult.EmployeeRank[0];
    }

    /**
     * For each factory, calculate the most profitable 4th product.
     *
     * @return QueryResult.FactoryProfit
     */
    public QueryResult.FactoryProfit calculateFourth() {
        return new QueryResult.FactoryProfit(0,0,0);
    }

    /**
     * Determine the salary variance between an employee and another
     * one who began working immediately after the first employee (by
     * startDate), for all employees.
     *
     * @return QueryResult.SalaryVariant[]
     */
    public QueryResult.SalaryVariant[] calculateVariance() {
        return new QueryResult.SalaryVariant[0];
    }

    /**
     * Create a method that is called once and whenever a Product starts
     * losing money, deletes it from Produce table
     *
     * @return void
     */
    public void deleteLosing() {

    }
}
