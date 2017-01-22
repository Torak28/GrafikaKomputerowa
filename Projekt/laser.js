function Laser(spos, angle) {
	this.pos = createVector(spos.x, spos.y);
	this.vel = p5.Vector.fromAngle(angle);
	this.vel.mult(10);

	this.update = function() {
		this.pos.add(this.vel);
	}
	
	this.render = function() {
		push();
		stroke(random(255), random(255), random(255));
		strokeWeight(6);
		ellipse(this.pos.x, this.pos.y, 5, 7);
		pop();
	}

	this.hits = function(asteroid) {
		var d = dist(this.pos.x, this.pos.y, asteroid.pos.x, asteroid.pos.y);
		if (d < asteroid.r) {
			return true;
		} else {
			return false;
		}
	}

	this.offscreen = function() {
		if (this.pos.x > width || this.pos.x < 0) {
			return true;
		}
		if (this.pos.y > height || this.pos.y < 0) {
			return true;
		}
		return false;
	}


}