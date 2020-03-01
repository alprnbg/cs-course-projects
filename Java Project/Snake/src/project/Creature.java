package project;

import game.Direction;
import game.Drawable;
import naturesimulator.Action;
import naturesimulator.LocalInformation;
import ui.GridPanel;
/**
 * Abstract class which is mother to Plant and Herbivore.
 * Implements Drawable interface.
 * Some methods are abstract, due to differences between Plants and Herbivores.
 * These abstract methods will be implemented in the class of the child.
 * 
 * @author cakmadam98
 *
 */
public abstract class Creature implements Drawable {
	
	/**
	 * Projection on the x-axis.
	 * It declared private to maintain data protection.
	 */
	private int x;
	/**
	 * Projection on the y-axis.
	 * It declared private to maintain data protection.
	 */
	private int y;
	/**
	 * Constructor which initializes the values of x and y.
	 * It uses super keyword to use the constructor of the Creature class.
	 * @param x x value of the creature
	 * @param y y value of the creature
	 */
	public Creature(int x, int y) {
		this.x=x;
		this.y=y;
	}
	/**
	 * Returns the projection of the creature on the y-axis
	 * @return the y-value of the creature.
	 */
	public int getY() {
		return y;
	}
	/**
	 * Returns the projection of the creature on the x-axis.
	 * @return returns the x-value of the creature.
	 */
	public int getX() {
		return x;
	}
	/**
	 * Helpful to change the x-value of the creature.
	 * @param x new x-coordinate of the creature.
	 */
	public void setX(int x) {
		this.x=x;
	}
	/**
	 * Helpful to change the y-value of the creature.
	 * @param y new y-coordinate of the creature.
	 */
	public void setY(int y) {
		this.y=y;
	}
	/**
	 * Abstract method which will be implemented in the child's class with more details.
	 * @return returns the current health of the creature.
	 */
	public abstract double getHealth();
	/**
	 * Abstract method which will be implemented in the child's class with more details.
	 * @param health new health of the creature.
	 */
	public abstract void setHealth(double health);
	/**
	 * Abstract method which will be implemented in the child's class with more details.
	 * @param createLocalInformationForCreature representing the information a creature has about its surroundings.
	 * @return returns the convenient action.
	 */
	public abstract Action chooseAction(LocalInformation createLocalInformationForCreature);
	/**
	 * Due to low health conditions, this method is called.
	 * It is implemented differently in the child's classes.
	 */
	public abstract void stay();
	/**
	 * Abstract method which will be implemented in the child's class with more details.
	 * @param direction creature reproduces in this direction.
	 * @return returns the new creature.
	 */
	public abstract Creature reproduce(Direction direction);
	/**
	 * Abstract method which will be implemented in the child's class with more details.
	 * @param direction the direction creature wants to go.
	 */
	public abstract void move(Direction direction);
	/**
	 * Abstract method which will be implemented in the child's class with more details.
	 * @param attackedCreature the creature that will be attacked.
	 */
	public abstract void attack(Creature attackedCreature);
	
	/**
	 * Abstract method which will be implemented in the child's class with more details.
	 * @param panel a grid panel that will be drawn on.
	 */
	public abstract void draw(GridPanel panel);
}
