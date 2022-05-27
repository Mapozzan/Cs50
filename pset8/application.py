import os

from cs50 import SQL
from flask import Flask, flash, jsonify, redirect, render_template, request, session
from flask_session import Session
from tempfile import mkdtemp
from werkzeug.exceptions import default_exceptions, HTTPException, InternalServerError
from werkzeug.security import check_password_hash, generate_password_hash

from helpers import apology, login_required, lookup, usd

# Configure application
app = Flask(__name__)

# Ensure templates are auto-reloaded
app.config["TEMPLATES_AUTO_RELOAD"] = True

# Ensure responses aren't cached
@app.after_request
def after_request(response):
    response.headers["Cache-Control"] = "no-cache, no-store, must-revalidate"
    response.headers["Expires"] = 0
    response.headers["Pragma"] = "no-cache"
    return response

# Custom filter
app.jinja_env.filters["usd"] = usd

# Configure session to use filesystem (instead of signed cookies)
app.config["SESSION_FILE_DIR"] = mkdtemp()
app.config["SESSION_PERMANENT"] = False
app.config["SESSION_TYPE"] = "filesystem"
Session(app)

# Configure CS50 Library to use SQLite database
db = SQL("sqlite:///finance.db")

# Make sure API key is set
if not os.environ.get("API_KEY"):
    raise RuntimeError("API_KEY not set")


@app.route("/")
@login_required
def index():
    """Show portfolio of stocks"""

    row = db.execute("SELECT * FROM stocks WHERE USER_id =:user",user=session["user_id"])
    user_cash = db.execute("SELECT cash FROM users WHERE id = :user",user=session["user_id"])[0]["cash"]
    user_name_1 = db.execute("SELECT username FROM users WHERE id = :user",user=session["user_id"])[0]["username"]

    return render_template("index.html",cash=round(user_cash,2), rows=row, user_name = user_name_1)


@app.route("/buy", methods=["GET", "POST"])
@login_required
def buy():
    """Buy shares of stock"""
    if request.method == "POST":

        stock = lookup(request.form.get("symbol"))
        share = int(request.form.get("shares"))
        if not stock:
             return apology("Provide a valid Stock", 403)
        if not share:
             return apology("Provide how many shares", 403)

        user_cash = db.execute("SELECT cash FROM users WHERE id = :user",user=session["user_id"])[0]["cash"]

        if (stock["price"]*float(share))>user_cash:
             return apology("Need more money", 403)

        rest_user_cash = user_cash - (stock["price"]*float(share))

        check_existing_share = db.execute("SELECT total_shares FROM stocks WHERE user_id = :user AND symbol = :symbol ",user=session["user_id"],symbol=stock["symbol"])

        if not check_existing_share:
            db.execute("INSERT INTO stocks(user_id,symbol,total_shares,name,price) VALUES (:user, :symbol, :total_shares, :name, :price)",user=session["user_id"],symbol=stock["symbol"],total_shares=share,name=stock["name"],price=stock["price"])
        else:
            total_shares = share +  check_existing_share[0]["total_shares"]
            db.execute("UPDATE stocks SET total_shares = :shares WHERE user_id = :user and symbol = :symbol",shares=total_shares, user=session["user_id"], symbol=stock["symbol"] )

        db.execute("UPDATE users SET cash = :cash WHERE id = :user",cash=rest_user_cash, user=session["user_id"] )
        db.execute("INSERT INTO historic(user_id,symbol,total_shares,price,type_op) VALUES (:user, :symbol, :total_shares, :price, :type_op)",user=session["user_id"],symbol=stock["symbol"],total_shares=share,price=stock["price"], type_op=1)

        flash("Got it!")
        return redirect("/")

    else:
        return render_template("buy.html")

@app.route("/history")
@login_required
def history():
    """Show history of transactions"""

    rows = db.execute("SELECT * FROM historic WHERE user_id = :user", user= session["user_id"])
    return render_template("history.html",rows=rows)


@app.route("/login", methods=["GET", "POST"])
def login():
    """Log user in"""

    # Forget any user_id
    session.clear()

    # User reached route via POST (as by submitting a form via POST)
    if request.method == "POST":

        # Ensure username was submitted
        if not request.form.get("username"):
            return apology("must provide username", 403)

        # Ensure password was submitted
        elif not request.form.get("password"):
            return apology("must provide password", 403)

        # Query database for username
        rows = db.execute("SELECT * FROM users WHERE username = :username",
                          username=request.form.get("username"))

        # Ensure username exists and password is correct
        if len(rows) != 1 or not check_password_hash(rows[0]["hash"], request.form.get("password")):
            return apology("invalid username and/or password", 403)

        # Remember which user has logged in
        session["user_id"] = rows[0]["id"]

        # Redirect user to home page
        return redirect("/")

    # User reached route via GET (as by clicking a link or via redirect)
    else:
        return render_template("login.html")


@app.route("/logout")
def logout():
    """Log user out"""

    # Forget any user_id
    session.clear()

    # Redirect user to login form
    return redirect("/")


@app.route("/quote", methods=["GET", "POST"])
@login_required
def quote():
    """Get stock quote."""
    if request.method =="POST":

        stock = lookup(request.form.get("symbol"))

        if not stock:
             return apology("Provide a Valid Stock", 403)

        return render_template("/quoted.html",stock=stock)
    else:
        return render_template("/quote.html")


@app.route("/register", methods=["GET", "POST"])
def register():

    session.clear()

    if request.method =="POST":

        if not request.form.get("username"):
            return apology("must provide username", 403)

        elif not request.form.get("password"):
            return apology("must provide password", 403)

        elif not request.form.get("confirmation"):
            return apology("must confirm password", 403)

        elif request.form.get("password") != request.form.get("confirmation"):
            return apology("Password don't match", 403)

        elif db.execute("SELECT * FROM users WHERE username = :username",username=request.form.get("username")):
            return apology("Users already exist", 403)

        db.execute("INSERT INTO users(username, hash) VALUES (:username,:hash)", username=request.form.get("username"), hash=generate_password_hash(request.form.get("password")) )

        # Query database for username
        rows = db.execute("SELECT * FROM users WHERE username = :username",username=request.form.get("username"))

        # Remember which user has logged in
        session["user_id"] = rows[0]["id"]

        # Redirect user to home page
        return redirect("/")

    else:
        return render_template("register.html")

@app.route("/sell", methods=["GET", "POST"])
@login_required
def sell():
    """Sell shares of stock"""
    if request.method == "POST":

        sell_stock = request.form.get("symbol")
        sell_share = int(request.form.get("shares"))
        price= round(lookup(sell_stock)["price"],2)

        if not sell_share:
             return apology("Provide how many shares", 403)

        if sell_share < 0:
            return apology("Provide how many shares", 403)

        user_cash = db.execute("SELECT cash FROM users WHERE id = :user",user=session["user_id"])[0]["cash"]
        check_existing_share = db.execute("SELECT total_shares FROM stocks WHERE user_id = :user AND symbol = :symbol ",user=session["user_id"],symbol=sell_stock)
        total_shares = check_existing_share[0]["total_shares"] - sell_share

        if sell_share > check_existing_share[0]["total_shares"]:
            return apology("You don't have all that", 403)
        elif (sell_share - check_existing_share[0]["total_shares"]) == 0:
            user_cash += float(sell_share)*price
            db.execute("DELETE FROM stocks Where user_id = :user AND symbol = :symbol", user =session["user_id"], symbol = sell_stock)
            db.execute("UPDATE users SET cash = :cash WHERE id = :user", cash = user_cash, user = session["user_id"])
        else:
            user_cash += float(sell_share)*price
            db.execute("UPDATE stocks SET total_shares = :shares WHERE user_id = :user and symbol = :symbol",shares=total_shares, user=session["user_id"], symbol= sell_stock )
            db.execute("UPDATE users SET cash = :cash WHERE id = :user", cash = user_cash, user = session["user_id"])

        db.execute("INSERT INTO historic(user_id,symbol,total_shares,price,type_op) VALUES (:user, :symbol, :total_shares, :price, :type_op)",user=session["user_id"],symbol=sell_stock, total_shares=sell_share, price=price, type_op=2)

        flash("Sold!")
        return redirect("/")

    else:
        owned_stocks = db.execute("SELECT symbol FROM stocks WHERE user_id = :user", user = session["user_id"])
        return render_template("sell.html",stocks=owned_stocks)


@app.route("/transfer", methods=["GET", "POST"])
@login_required
def transfer():
    "Allow user to add additional cash"

    if request.method == "POST":
        add_cash = int(request.form.get("cash"))
        if add_cash <= 50:
             return apology("Need more money", 403)
        else:
            user_cash = db.execute("SELECT cash FROM users WHERE id = :user",user=session["user_id"])[0]["cash"]
            total_cash = float(add_cash) + user_cash
            db.execute("UPDATE users SET cash = :cash WHERE id = :user", cash = total_cash, user = session["user_id"])
        return redirect("/")
    else:
        return render_template("transfer.html")


def errorhandler(e):
    """Handle error"""
    if not isinstance(e, HTTPException):
        e = InternalServerError()
    return apology(e.name, e.code)


# Listen for errors
for code in default_exceptions:
    app.errorhandler(code)(errorhandler)
