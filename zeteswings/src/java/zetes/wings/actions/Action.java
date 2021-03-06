package zetes.wings.actions;

import java.util.HashMap;
import java.util.Map;


import zetes.wings.HotKey;
import zetes.wings.abstracts.ViewWindow;
import zetes.wings.actions.Action;
import zetes.wings.actions.ActionHierarchyMember;
import zetes.wings.actions.Handler;


public class Action<TVW extends ViewWindow<?>> extends ActionHierarchyMember<TVW> {
	
	private HotKey hotKey;
	private String title;

	private HashMap<TVW, Handler<TVW>> handlers = new HashMap<>();
	
	public Action() {
		
	}
	
	public Action(String title) {
		this.title = title;
	}
	
	public HotKey getHotKey() {
		return hotKey;
	}
	public void setHotKey(HotKey hotKey) {
		this.hotKey = hotKey;
	}
	
	public String getTitle() {
		return title;
	}

	public void setTitle(String title) {
		this.title = title;
	}

	
	/**
	 * <p>You can set an action handler for every window. If you want to set
	 * the default handler (which isn't connected to any window), use the key {@code null}.</p>
	 * 
	 * <p>If default handler is specified for the {@link Action}, this action is shown for every
	 * window (and in the case when no windows are present on OS X).</p>
	 * <p>If a {@link Handler} is specified for the active {@link ViewWindow}, it overrides the
	 * default one.</p>
	 * <p>If no default {@link Handler} specified, the action should be shown only for
	 * windows that provide a specific handler for the {@link Action}</p>
	 * 
	 * @return A modifiable {@link Map} that connects view windows and handlers
	 */
	public Map<TVW, Handler<TVW>> getHandlers() {
		return handlers;
	}

}
