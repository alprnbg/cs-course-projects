var express = require('express'),
	path = require('path'),
	bodyParser = require('body-parser'),
	cons = require('consolidate'),
	dust = require('dustjs-helpers'),
	pg = require('pg'),
	app = express(),
	cookieParser = require("cookie-parser"),
	session = require('express-session');


const connect = 'postgresql://log_role:12345@localhost/db_321';

app.engine('dust', cons.dust);
app.set('view engine', 'dust');
app.set('views', __dirname+'/views');
app.use(express.static(path.join(__dirname, 'public')));
app.use(bodyParser.json());
app.use(bodyParser.urlencoded({ extended: false }));

app.use(session({
  secret: 'secret-key',
  resave: false,
  saveUninitialized: true
}));



app.post('/login', function (req, res) {
	pg.connect(connect, function(err, client, done) {
		if(err){
			return console.error('error fetching client from pool', err);
		}
		client.query('SELECT username FROM public."Managers" WHERE username = $1 AND password = $2',
			[req.body.username, req.body.password],
			function(err, result) {
				if(err){
					return res.render("please_login");
				}
				if (result.rowCount == 1){
					req.session.username = req.body.username;
					done();
					res.redirect('/');
				}else{
					return res.render("please_login");
				}
			});
	});
});

app.get('/', function(req, res){
	if (req.session.username){
		res.render('index');
	}else{
		res.render('please_login');
	}
});


////////////////////////////////////////////////////// MANAGER //////////////////////////////////////////////////////


app.post('/add_manager', function(req, res){ 
	pg.connect(connect, function(err, client, done) {
		if(err){
			return console.error('error fetching client from pool', err);
		}
		client.query('INSERT INTO public."Managers"(username, password) VALUES($1, $2)',
			[req.body.username, req.body.password],
			function(err, result) {
				if(err){
					return res.render("/");
				}
			});
			done();
			res.redirect('/');
	});
});


app.post('/update_manager', function(req, res){  
	pg.connect(connect, function(err, client, done) {
		if(err){
			return console.error('error fetching client from pool', err);
		}
		client.query('UPDATE public."Managers" SET username = $1, password = $2 WHERE username = $3',
			[req.body.new_username, req.body.new_password, req.body.old_username], 
			function(err, result) {
				if(err){
					return res.render("/");
				}
			});
			done();
			res.redirect('/');
	});
});


app.post('/delete_manager', function(req, res){  
	pg.connect(connect, function(err, client, done) {
		if(err){
			return console.error('error fetching client from pool', err);
		}
		client.query('DELETE FROM public."Managers" WHERE username = $1',
			[req.body.username], 
			function(err, result) {
				if(err){
					return res.render("/");
				}
			});
			done();
			res.redirect('/');
	});
});


app.get('/view_managers', function(req, res){  
	pg.connect(connect, function(err, client, done) {
		if(err){
			return console.error('error fetching client from pool', err);
		}
		client.query('SELECT * FROM public."Managers"',
			function(err, result) {
				if(err){
					return res.render("/");
				}
				res.render('managers', {managers: result.rows});
				done();
			});
	});
});

////////////////////////////////////////////////////// CARETAKER //////////////////////////////////////////////////////


app.post('/add_caretaker', function(req, res){  
	pg.connect(connect, function(err, client, done) {
		if(err){
			return console.error('error fetching client from pool', err);
		}
		client.query('INSERT INTO public."Caretakers"(name, surname) VALUES($1, $2)',
			[req.body.name, req.body.surname],
			function(err, result) {
				if(err){
					return res.render("/");
				}
			});
			done();
			res.redirect('/');
	});
});


app.post('/update_caretaker', function(req, res){  
	pg.connect(connect, function(err, client, done) {
		if(err){
			return console.error('error fetching client from pool', err);
		}
		client.query('UPDATE public."Caretakers" SET name = $1, surname = $2 WHERE id = $3',
			[req.body.new_name, req.body.new_surname, req.body.id], 
			function(err, result) {
				if(err){
					return res.render("/");
				}
			});
			done();
			res.redirect('/');
	});
});


app.post('/delete_caretaker', function(req, res){  
	pg.connect(connect, function(err, client, done) {
		if(err){
			return console.error('error fetching client from pool', err);
		}
		client.query('DELETE FROM public."Caretakers" WHERE id = $1',
			[req.body.id], 
			function(err, result) {
				if(err){
					return res.render("/");
				}
			});
			done();
			res.redirect('/');
	});
});

app.get('/view_caretakers', function(req, res){  
	pg.connect(connect, function(err, client, done) {
		if(err){
			return console.error('error fetching client from pool', err);
		}
		client.query('SELECT * FROM public."Caretakers"',
			function(err, result) {
				if(err){
					return res.render("/");
				}
				res.render('caretakers', {caretakers: result.rows});
				done();
			});
	});
});

app.get('/rank_caretakers', function(req, res){  
	pg.connect(connect, function(err, client, done) {
		if(err){
			return console.error('error fetching client from pool', err);
		}
		client.query('SELECT caretaker, amount FROM (SELECT COUNT(id) AS amount, caretaker FROM public."Animals" GROUP BY caretaker ORDER BY amount DESC) AS d;',
			function(err, result) {
				if(err){
					return res.render("/");
				}
				res.render('caretakers_rank', {caretakers: result.rows});
				done();
			});
	});
});

////////////////////////////////////////////////////// ANIMAL //////////////////////////////////////////////////////


app.post('/add_animal', function(req, res){  
	pg.connect(connect, function(err, client, done) {
		if(err){
			return console.error('error fetching client from pool', err);
		}
		client.query('INSERT INTO public."Animals"(name, age, species, caretaker, sponsor) VALUES($1, $2, $3, $4, $5)',
			[req.body.name, req.body.age, req.body.species, null, req.body.sponsor_id || null],
			function(err, result) {
				console.log(err);
				if(err){
					return res.render("/");
				}
			});
			done();
			res.redirect('/');
	});
});


app.post('/update_animal', function(req, res){  
	pg.connect(connect, function(err, client, done) {
		if(err){
			return console.error('error fetching client from pool', err);
		}
		client.query('UPDATE public."Animals" SET name = $1, age = $2, species = $3, caretaker = $4, sponsor = $5 WHERE id = $6',
			[req.body.new_name, req.body.new_age, req.body.new_species, req.body.new_caretaker_id, req.body.new_sponsor_id || null, req.body.id],
			function(err, result) {
				if(err){
					return res.render("/");
				}
			});
			done();
			res.redirect('/');
	});
});


app.post('/delete_animal', function(req, res){  
	pg.connect(connect, function(err, client, done) {
		if(err){
			return console.error('error fetching client from pool', err);
		}
		client.query('DELETE FROM public."Animals" WHERE id = $1',
			[req.body.id], 
			function(err, result) {
				if(err){
					return res.render("/");
				}
			});
			done();
			res.redirect('/');
	});
});

app.get('/view_animals', function(req, res){  
	pg.connect(connect, function(err, client, done) {
		if(err){
			return console.error('error fetching client from pool', err);
		}
		client.query('SELECT * FROM public."Animals"',
			function(err, result) {
				if(err){
					return res.render("/");
				}
				res.render('animals', {animals: result.rows});
				done();
			});
	});
});


////////////////////////////////////////////////////// SPONSOR //////////////////////////////////////////////////////

app.post('/add_sponsor', function(req, res){  
	pg.connect(connect, function(err, client, done) {
		if(err){
			return console.error('error fetching client from pool', err);
		}
		client.query('INSERT INTO public."Sponsors"(name, surname, phone) VALUES($1, $2, $3)',
			[req.body.name, req.body.surname, req.body.phone],
			function(err, result) {
				if(err){
					return res.render("/");
				}
			});
			done();
			res.redirect('/');
	});
});


app.post('/update_sponsor', function(req, res){  
	pg.connect(connect, function(err, client, done) {
		if(err){
			return console.error('error fetching client from pool', err);
		}
		client.query('UPDATE public."Sponsors" SET name = $1, surname = $2, phone = $3 WHERE id = $4',
			[req.body.new_name, req.body.new_surname, req.body.new_phone, req.body.id],
			function(err, result) {
				if(err){
					return res.render("/");
				}
			});
			done();
			res.redirect('/');
	});
});


app.post('/delete_sponsor', function(req, res){  
	pg.connect(connect, function(err, client, done) {
		if(err){
			return console.error('error fetching client from pool', err);
		}
		client.query('DELETE FROM public."Sponsors" WHERE id = $1',
			[req.body.id], 
			function(err, result) {
				if(err){
					return res.render("/");
				}
			});
			done();
			res.redirect('/');
	});
});

app.get('/view_sponsors', function(req, res){  
	pg.connect(connect, function(err, client, done) {
		if(err){
			return console.error('error fetching client from pool', err);
		}
		client.query('SELECT * FROM public."Sponsors"',
			function(err, result) {
				if(err){
					return res.render("/");
				}
				res.render('sponsors', {sponsors: result.rows});
				done();
			});
	});
});


////////////////////////////////////////////////////// SEARCH //////////////////////////////////////////////////////


app.post('/search_by_caretaker', function(req, res){  
	pg.connect(connect, function(err, client, done) {
		if(err){
			return console.error('error fetching client from pool', err);
		}
		client.query('SELECT * FROM public."Animals" WHERE caretaker = $1',
			[req.body.caretaker_id], 
			function(err, result) {
				if(err){
					return res.render("/");
				}
				res.render('animals', {animals: result.rows});
				done();
			});
	});
});


app.post('/search_by_sponsor', function(req, res){  
	pg.connect(connect, function(err, client, done) {
		if(err){
			return console.error('error fetching client from pool', err);
		}
		client.query('SELECT * FROM public."Animals" WHERE sponsor = $1',
			[req.body.sponsor_id], 
			function(err, result) {
				if(err){
					return res.render("/");
				}
				res.render('animals', {animals: result.rows});
				done();
			});
	});
});


app.post('/search_by_species', function(req, res){  
	pg.connect(connect, function(err, client, done) {
		if(err){
			return console.error('error fetching client from pool', err);
		}
		client.query('SELECT * FROM search_by_species($1)',
			[req.body.species], 
			function(err, result) {
				if(err){
					return res.render("/");
				}
				res.render('animals_search', {animals: result.rows});
				done();
			});
	});
});



app.get('/animals_without_sponsor', function(req, res){  
	pg.connect(connect, function(err, client, done) {
		if(err){
			return console.error('error fetching client from pool', err);
		}
		client.query('SELECT * FROM public."Animals" WHERE sponsor is NULL',
			function(err, result) {
				if(err){
					return res.render("/");
				}
				res.render('animals', {animals: result.rows});
				done();
			});
	});
});


app.listen(3000, function(){
	console.log('server started on port 3');
});

